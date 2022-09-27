/*
 * Copyright 2017 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "src/binary-reader.h"
#include "src/binary-reader-ir.h"
// #include "src/error-formatter.h"
#include "src/ir.h"
#include "src/option-parser.h"
#include "src/stream.h"
#include "utilities/u_utils.h" // basics utilities
#include "encoder/u_encoder.h"
#include "encoder/u_inst_encoder.h"
#include "encoder/u_type_encoder.h"
#include "converter.h"

using namespace wabt;

#define COMMAND_MAX 1024

static int s_verbose;
static std::string s_infile;
static std::string s_outfile;
static Features s_features;
static bool s_read_debug_names = true;
static bool s_fail_on_custom_section_error = true;
static bool s_dis = false;
static std::unique_ptr<FileStream> s_log_stream;
static int s_data_bit_mode = 64;
static int s_compile_only_mode = false;
static INT64 s_start_func_id = 0;
static bool s_start_func_set = false;
static INT64 s_maximum_func_id = 0;
static bool s_maximum_limited = false;
static bool s_stack_check = false;

static const char s_description[] =
R"(  Read a file in the WebAssembly binary format,
     then transform to uWASM, then start a debugger to run the uWASM.

examples:
  # to convert wasm and run the file test.wasm
  $ wasm_to_uwasm -t 5 -o test.uwm test.wasm
)";

/**
 * Check if the string has an ending like xxxxxabc, given 'abc'
 * @param fullString
 * @param ending
 * @return
 */
static bool String_has_ending (std::string const &fullString, std::string const &ending) {
  if (fullString.length() >= ending.length()) {
    return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
  } else {
    return false;
  }
}

static void ParseOptions(int argc, char** argv) {
  OptionParser parser("wasm_to_uwasm", s_description);

  parser.AddOption('v', "verbose", "Use multiple times for more info", []() {
    s_verbose++;
    s_log_stream = FileStream::CreateStdout();
    Set_tracing_opt(TP_CONV, TRACE_ENABLE_LEVEL);
    Set_conv_trace(TCK_ANY, TRACE_ENABLE_LEVEL);
  });

  s_features.AddOptions(&parser);
  parser.AddOption("ignore-custom-section-errors",
                   "Ignore errors in custom sections",
                   []() { s_fail_on_custom_section_error = false; });

  parser.AddOption( 't', "trace", "TRACE_LEVEL",
                   "Enable tracing at specific level 0-4",
                   [](const char* argument) {
                       UINT64 trace_level = strtol(argument, NULL, 10);
                       Set_conv_trace((TRACE_CONV_KIND) trace_level, TRACE_ENABLE_LEVEL);
                   });
  parser.AddOption( 'f', "startfuncid", "START_BY_FUNC_ID",
                    "Skip the function by starting at a specific id",
                    [](const char* argument) {
                        s_start_func_id = strtol(argument, NULL, 10);
                        s_start_func_set = true;
                    });
  parser.AddOption( 'f', "maxfuncs", "START_BY_FUNC_LENGTH",
                    "Skip the function by starting at a specific id",
                    [](const char* argument) {
                        s_maximum_func_id = strtol(argument, NULL, 10);
                        s_maximum_limited = true;
                    });

  parser.AddOption(
    'o', "output", "FILENAME",
    "Output file for the generated wast file, by default use stdout",
    [](const char* argument) {
      s_outfile = argument;
      ConvertBackslashToSlash(&s_outfile);
      Is_True(s_outfile.c_str() != nullptr, ("Argument should not be null"));
      Get_conv_opt().Set_output_name(s_outfile.c_str());
    });

  parser.AddOption("m32", "(NYI) Data mode whether to enable mode of 32-bit addressing", []() {
      s_data_bit_mode = 32;
  });

  parser.AddOption("stackchk", "Enable end of function stack size verification", []() {
    s_stack_check = true;
  });

  parser.AddOption("m64", "(NYI) Data mode whether to enable mode of 64-bit addressing", []() {
      s_data_bit_mode = 64;
  });

  parser.AddOption('D', "DEFINE", "STRING", "Add definition params", [](const char *value) {});
  parser.AddOption("noxa", "(NYI) Enable IPA analysis, used for compatibility only", []() {});
  parser.AddOption("noxfa", "(NYI) Disable Cross file analysis, compatibility use only", []() {});
  parser.AddOption("json", "(NYI) Disable Cross file analysis, compatibility use only", []() {});
  parser.AddOption("Ofast", "Set opt level to Ofast, compatibility use only", []() {});
  parser.AddOption("O3", "Set opt level to O3, compatibility use only", []() {});
  parser.AddOption("O2", "Set opt level to O2, compatibility use only", []() {});
  parser.AddOption("O1", "Set opt level to O1, compatibility use only", []() {});
  parser.AddOption("O0", "Set opt level to O0, compatibility use only", []() {});
  parser.AddOption('O', "Opt", "INTEGER", "Set opt level to O0, compatibility use only", [](const char *value) {
  });
  parser.AddOption("c", "Set flag to control for compile only mode, used for compat",
                   []() {
    s_compile_only_mode = true;
  });

  parser.AddArgument("filename", OptionParser::ArgumentCount::One,
                     [](const char* argument) {
                       s_infile = argument;
                       ConvertBackslashToSlash(&s_infile);
                     });
  parser.Parse(argc, argv);
}

using IMPVEC = std::vector<Import*>;
using IMPIT = std::vector<Import*>::iterator;

NTYPE Conv_valty_to_nty (VALTYPE valty) {
  switch(valty) {
    case I32: { return NTYPE_I4; }
    case I64: { return NTYPE_I8; }
    case F32: { return NTYPE_F4; }
    case F64: { return NTYPE_F8; }
    default: {
      Is_True_Rel(false, ("Cannot convert valty = %d", valty));
    }
  }
}

UINT32 Process_external_func(U_ENCODER &encoder, const std::string &combined_name, const FuncSignature *funcdesc) {
  (funcdesc != nullptr, ("funcdesc should not be a null pointer, see wasm spec P17 for details."));
  vector<NTYPE> param_ty_list;
  vector<VALTYPE> valty_list, ret_ty_list;
  NTYPE ret = NTYPE::NTYPE_V;
  if (funcdesc->GetNumResults() == 1) {
    VALTYPE res = Conv_wabt_ty(funcdesc->GetResultType(0));
    ret_ty_list.push_back(res);
    ret = Conv_valty_to_nty(res);
  } else if (funcdesc->GetNumResults() > 0) {
    Is_True(false, ("Incorrect number of results = %d for import %s.",
                    funcdesc->GetNumResults(), combined_name.c_str()));
  }
  if (funcdesc->GetNumParams() > 0) {
    for (INT32 i = 0; i < funcdesc->GetNumParams(); ++i) {
      VALTYPE res = Conv_wabt_ty(funcdesc->GetParamType(i));
      valty_list.push_back(res);
      param_ty_list.push_back(Conv_valty_to_nty(res));
    }
  }
  UINT32 type_idx = encoder.Add_type(ret_ty_list, valty_list);
  UINT32 i = encoder.Request_native_func(combined_name.c_str(), type_idx, ret, param_ty_list);
  return i;
}


void Process_wasm_exports(U_ENCODER &encoder, FILE_CONTEXT &ctx) {
  vector<Export *> &exports = ctx.Get_module()->exports;
  for (UINT32 i = 0; i < exports.size(); i++) {
    Export *export_info= exports.at(i);
    switch (export_info->kind) {
      case ExternalKind::Func: {
        Is_Trace(Convtrace(TCK_MAIN),
                 (TFile, "Wasm function export found: %d -> %s\n",
                   export_info->var.index(),
                   export_info->name.c_str()));
        ctx.Set_func_name_cache(export_info->var.index(), export_info->name);
        break;
      }
      case ExternalKind::Global: {
        ctx.Set_global_name_cache(export_info->var.index(), export_info->name);
        break;
      }
      default: {
        Is_Trace(Convtrace(TCK_MAIN),
                 (TFile, "NYI, global not processed, kind = %d, name = %s\n",
                 (INT32) export_info->kind, export_info->name.c_str()));
      }
    }
  }
}


void Process_wasm_imports(U_ENCODER &encoder, FILE_CONTEXT &ctx) {
  IMPVEC &imports = ctx.Get_module()->imports;
  for(IMPIT iter = imports.begin(); iter != imports.end(); iter++) {
    Import &import_item = *(*iter);
    if (import_item.kind() == ExternalKind::Func) {
      // add a external function
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "found import with kind = function, "
                                         "module = %s, name = %s \n",
                                  import_item.module_name.c_str(),
                                  import_item.field_name.c_str()));
      FuncImport *func_import = reinterpret_cast<FuncImport *>(&import_item);
      string combined_name = import_item.field_name;
      Func *funcdesc = &func_import->func;
      Process_external_func(encoder, combined_name, &funcdesc->decl.sig);
    } else if (import_item.kind() == ExternalKind::Global) {
      // add a external global
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "found import with kind = global, "
                                         "module = %s, name = %s, ",
        import_item.module_name.c_str(),
        import_item.field_name.c_str()));
      UINT32 cur_wasm_id = ctx.Get_import_global_cnt();
      GlobalImport *global_import = reinterpret_cast<GlobalImport *>(&import_item);
      string combined_name = import_item.module_name + "." + import_item.field_name;
      UINT32 global_id = encoder.Add_global(combined_name.c_str());
      ctx.Set_global_name_cache(cur_wasm_id, combined_name);
      ctx.Set_global_id_map(cur_wasm_id, global_id);
      ctx.Inc_import_global_cnt();
      Is_True(cur_wasm_id + 1 == global_id,
              ("Inconsistent global idx mapping, wasm id = %d, "
               "yet the uwasm-global_id = %d, uwasm should be wasm + 1",
               cur_wasm_id, global_id));
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "uwasm-global id = %d, wasm id = %d\n",
                                     global_id, cur_wasm_id));
      //Is_True(false, ("%s, found import with kind = global", NYI));
    } else if (import_item.kind() == ExternalKind::Table) {
      //Is_True(false, ("%s, found import with kind = table", NYI));
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "%s, found import with kind = table\n", NYI));
    } else if (import_item.kind() == ExternalKind::Memory) {
      //Set memory to be at some position.
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "%s, found import with kind = memory\n", NYI));
    } else {
      Is_True(false, ("%s, found import with kind = %d", NYI, import_item.kind()));
    }
  }
}

void Process_wasm_globals(U_ENCODER &encoder, FILE_CONTEXT &ctx) {
  typedef vector<Global *>::iterator GLBLIT;
  UINT32 wasm_id = 0;
  UINT32 import_globals = ctx.Get_import_global_cnt();
  for(GLBLIT iter = ctx.Get_module()->globals.begin(); iter != ctx.Get_module()->globals.end(); iter++, wasm_id++) {
    Global *glbl = *iter;
    string anon_global_name;
    const char *global_name = glbl->name.c_str();
    VALTYPE valty = Conv_wabt_ty(glbl->type);
    if (ctx.Get_global_name_cache(wasm_id) != nullptr) {
      global_name = ctx.Get_global_name_cache(wasm_id);
    } else if (glbl->name.size() == 0) {
      anon_global_name = ctx.Get_anon_global_name();
      global_name = anon_global_name.c_str();
    }
    if (wasm_id < import_globals) {
      Is_Trace(Convtrace(TCK_MAIN),
               (TFile, "Skipping wasm imported global entry (%d)['%s']\n",
                 wasm_id, global_name));
      continue;
    }
    ExprList &exprs = glbl->init_expr;
    Is_True_Warn(exprs.size() == 1, ("Incorrect number of expr for the global\n"));
    UINT64 init_value = 0;
    if (exprs.size() == 1) {
      wabt::Expr &expr_val = *(exprs.begin());
      Is_True_Warn(expr_val.type() == ExprType::Const,
                   ("Incorrect kind = %d of expr in the "
                    "init value of global\n", expr_val.type()));
      if (expr_val.type() == ExprType::Const) {
        ConstExpr &const_expr = reinterpret_cast<ConstExpr &>(expr_val);
        Is_True(Conv_wabt_ty(const_expr.const_.type()) == valty, ("Constant type not matching the global's type"));
        switch (valty) {
          case I32: init_value = const_expr.const_.u32(); break;
          case I64: init_value = const_expr.const_.u64(); break;
          case F32: init_value = const_expr.const_.f32_bits(); break;
          case F64: init_value = const_expr.const_.f64_bits(); break;
          default: {
            Is_True_Rel(false, ("Unknown valtype"));
          }
        }
      }
    }
    UINT32 global_id = encoder.Add_global(glbl->mutable_, init_value, global_name);
    ctx.Set_global_id_map(wasm_id, global_id);
    Is_Trace(Convtrace(TCK_MAIN),
             (TFile, "Converting wasm global entry (%d)['%s'], to uwasm global id = %d\n",
               wasm_id, global_name, global_id));
  }
}


void Process_wasm_static_data(U_ENCODER &encoder, FILE_CONTEXT &ctx) {
  vector<DataSegment *> &dataseg = ctx.Get_module()->data_segments;
  UINT32 dseg_size = 0;
  UINT32 total_size = 0;
  for (UINT32 i = 0; i < dataseg.size(); i++) {
    DataSegment *data_chunk= dataseg.at(i);
    switch (data_chunk->kind) {
      case SegmentKind::Active: {
        Is_True_Rel(data_chunk->offset.size() == 1,
                    ("Active data segment has offset size != 1, size = %d",
                      data_chunk->offset.size()));
        Expr &ref = data_chunk->offset.back();
        Is_True_Rel(ref.type() == ExprType::Const,
                    ("Active data segment has offset that is not constant, yet type = %d", ref.type()));
        ConstExpr &offset = reinterpret_cast<ConstExpr &>(ref);
        Is_Trace(Convtrace(TCK_MAIN),
                 (TFile, "Active segment of data found "
                         "ofst = %u, size = %lu\n",
                  (UINT32) offset.const_.u32(),
                  data_chunk->data.size()));
        total_size += data_chunk->data.size();
        dseg_size++;
        break;
      }
      case SegmentKind::Declared: {
        Is_True_Rel(false, ("%s, Active segment of data found, not processed. ofst (expr.size = %lu), size = %lu", NYI,
          data_chunk->offset.size(), data_chunk->data.size()));
        break;
      }
      case SegmentKind::Passive: {
        Is_True_Rel(false, ("%s, Active segment of data found, not processed. ofst (expr.size = %lu), size = %lu", NYI,
          data_chunk->offset.size(), data_chunk->data.size()));
        break;
      }
      default: {
        Is_Trace(Convtrace(TCK_MAIN),
                 (TFile, "NYI, other data segment not processed"));
      }
    }
  }


  vector<UINT8> data_infos;

  // PREPEND THE DESEGS to the data segments
  UBH_DATA_SEGMENTS segs;
  UINT32 prepend_size = segs.Calc_size(dseg_size);
  total_size += prepend_size;
  data_infos.resize(total_size);

  // Initialize the ubh_data_seg info
  memcpy(data_infos.data(), &segs, sizeof(UBH_DATA_SEGMENTS));
  UBH_DATA_SEGMENTS &target_seg = *(UBH_DATA_SEGMENTS *) data_infos.data();
  target_seg.Set_size(dseg_size);

  // Resize data_infos.
  UINT32 current_copied = prepend_size;
  UINT32 index = 0;
  for (UINT32 i = 0; i < dataseg.size(); i++) {
    DataSegment *data_chunk = dataseg.at(i);
    switch (data_chunk->kind) {
      case SegmentKind::Active: {
        Is_True(current_copied + data_chunk->data.size() <= total_size,
                ("Data segment length exceeded, total = %lu, copied = %lu, cur_size = %lu",
                  total_size, current_copied, data_chunk->data.size()));
        Expr &ref = data_chunk->offset.back();
        Is_True_Rel(ref.type() == ExprType::Const,
                    ("Active data segment has offset that is not constant, yet type = %d", ref.type()));
        ConstExpr &offset = reinterpret_cast<ConstExpr &>(ref);
        memcpy(data_infos.data() + current_copied,
               data_chunk->data.data(),
               data_chunk->data.size());
        target_seg.Get_seg(index).Set_size(data_chunk->data.size());
        target_seg.Get_seg(index).Set_mem_ofs(offset.const_.u32());
        current_copied += data_chunk->data.size();
        index++;
        break;
      }
    }
  }
  Is_True(index == dseg_size && total_size == current_copied, ("Insufficient items copied."));
  Is_Trace(Convtrace(TCK_MAIN),
           (TFile, "Data segments converted with %u entries, "
                 "prepend_size = %u, total size = %u, magic = %.10s \n",
                 dseg_size, prepend_size, total_size, (const char *) data_infos.data()));
  // Add data segment
  encoder.Set_data_section(data_infos.size(), (char *) data_infos.data());
  // Done.

  if (ctx.Get_module()->memories.size() > 0) {
    Is_Trace(Convtrace(TCK_MAIN),
             (TFile, "Found memory info with limit: initial:%lu, max:%lu, has_max:%d, is_shared:%d \n",
               ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.initial,
               ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.max,
               ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.has_max,
               ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.is_shared));
//    encoder.Set_memory_seg(ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.initial,
//                           ctx.Get_module()->GetMemory(wabt::Var(0))->page_limits.max);
  }
}


int ProgramMain(int argc, char** argv) {
  Result result;
  Set_tracing_opt(TP_CONV, TRACE_ENABLE_LEVEL);

  InitStdio();
  ParseOptions(argc, argv);

  std::vector<uint8_t> file_data;
  result = ReadFile(s_infile.c_str(), &file_data);
  U_ENCODER encoder; // uwasm module
  if (Succeeded(result)) {
    Errors errors;
    Module module;
    const bool kStopOnFirstError = true;
    ReadBinaryOptions options(s_features, s_log_stream.get(),
                              s_read_debug_names, kStopOnFirstError,
                              s_fail_on_custom_section_error);
    FILE_CONTEXT ctx;
    ctx.Set_kind(FILE_WASM);
    ctx.Set_stack_check(s_stack_check);
    Get_conv_opt().Set_conv_in_read(false);

    // Set output file name, either uwm when linking or .o when -c is present.
    if (String_has_ending(s_infile, ".o")) {
      // linking mode, copy uwm to destination
      std::ifstream  src(s_infile.c_str(), std::ios::binary);
      std::ofstream  dst(s_outfile.c_str(),   std::ios::binary);
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "Copying '%s' to '%s'\n", s_infile.c_str(), s_outfile.c_str()));
      dst << src.rdbuf();
      return 0;
    } else if (s_compile_only_mode) {
      std::string newfilename = s_infile.substr(0, s_infile.find_last_of('.')) + ".o";
      Get_conv_opt().Set_output_name(newfilename.c_str());
    } else {
      std::string newfilename = s_infile.substr(0, s_infile.find_last_of('.')) + ".uwm";
      Get_conv_opt().Set_output_name(newfilename.c_str());
    }
    result = ReadBinaryIr(s_infile.c_str(), file_data.data(), file_data.size(),
                          options, &errors, &module);
    ctx.Set_module(module);
    if (Succeeded(result)) {
      // Process all imports
      Process_wasm_imports(encoder, ctx);
      Process_wasm_exports(encoder, ctx);
      Process_wasm_globals(encoder, ctx);
      Process_wasm_static_data(encoder, ctx);
      // Process all functions.
      ctx.Set_func_ctx(0);
      wabt::Func *func = ctx.Get_func();
      // Process the import section / global export section.
      while (func) {
        string anon_func_name;
        const char *func_name = func->name.c_str();
        if (ctx.Get_func_name_cache(ctx.Get_func_idx()) != nullptr) {
          func_name = ctx.Get_func_name_cache(ctx.Get_func_idx());
        } else if (func->name.size() == 0) {
          anon_func_name = ctx.Get_anon_func_name();
          func_name = anon_func_name.c_str();
        }
        if (func->GetWasmBin() == NULL) {
          Is_Trace(Convtrace(TCK_MAIN),
                   (TFile, "Skipping func : (%d)[%s], bin = 0x%016llx, size = %lu\n",
                     ctx.Get_func_idx(), func_name, (UINT64) func->GetWasmBin(), func->GetWasmBinSize()));
          func = ctx.Next_func();
          continue;
        }
        if (s_start_func_set && ctx.Get_func_idx() < s_start_func_id) {
          Is_Trace(Convtrace(TCK_MAIN),
                   (TFile, "Creating fake-debug func: (%d) name = '%s', bin = 0x%016llx, size = %lu\n",
                     ctx.Get_func_idx(), func_name, (UINT64) func->GetWasmBin(), func->GetWasmBinSize()));
          Process_external_func(encoder, func_name, &func->decl.sig);
          func = ctx.Next_func();
          continue;
        }
        if (s_maximum_limited && ctx.Get_func_idx() >= s_maximum_func_id) {
          Is_Trace(Convtrace(TCK_MAIN),
                   (TFile, "Ending the creation before func: (%d) name = '%s', bin = 0x%016llx, size = %lu\n",
                     ctx.Get_func_idx(), func_name, (UINT64) func->GetWasmBin(), func->GetWasmBinSize()));
          return 0;
        }
        Is_Trace(Convtrace(TCK_MAIN),
                 (TFile, "Converting func : (%d) name = '%s', bin = 0x%016llx, size = %lu\n",
                   ctx.Get_func_idx(), func_name, (UINT64) func->GetWasmBin(), func->GetWasmBinSize()));
        Is_True(func->GetWasmBin() != NULL, ("Wasm function binary input not found func = %p, wasm_bin = %p", func, func->GetWasmBin()));
        WINSVEC wasm_inst = Read_insts(func->GetWasmBin(), func->GetWasmBinSize(), ctx);
        Wasm_to_uwasm(encoder, wasm_inst, func->GetWasmBin(), func->GetWasmBinSize(), ctx);
        func = ctx.Next_func();
      }
      Is_True(Get_conv_opt().Get_output_name() != nullptr, ("No output file name given."));
      Is_Trace(Convtrace(TCK_MAIN), (TFile, "Output to file : %s \n", Get_conv_opt().Get_output_name()));
      encoder.Write_to_file(Get_conv_opt().Get_output_name());
      return 0;
    }
    // FormatErrorsToFile(errors, Location::Type::Binary);
    Is_True(false, ("Error occurred during wasm file reading."));
    exit(1);
  }
  return result != Result::Ok;
}

int main(int argc, char** argv) {
  WABT_TRY
  return ProgramMain(argc, argv);
  WABT_CATCH_BAD_ALLOC_AND_EXIT
}
