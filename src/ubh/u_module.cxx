/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by lishijie on 9/22/20.
//
#include "u_module.h"
#include "interpreter/exec_debug.h"
#include "ubh/ubh.h"
#include "u_inst.h"
#include "u_opcode.h"
#include <sys/mman.h>
#include <sys/errno.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef CHAR_BIT
#define CHAR_BIT 8u
#endif

extern const char *inst_impl_func_name[];
extern const char *Get_raw_type_name(UINT8 type);

void AM_EXPAND::Print(FILE *file) const {
  const UINT8 opcode = (_opcode >> (UINT8) VALTY_SZ);
  fprintf(file, "[INST]          %s", inst_impl_func_name[_opcode]);
  const TY_AM am_ty = UWINST_TY_AM[opcode];
  if (am_ty == TY_AM32_OFS) {
    fprintf(file, " (%d)\n", Get_ofs());
    return;
  }
  if (am_ty == TY_AM32) {
    fprintf(file, "\n");
    return;
  }
  // print first operand
  fprintf(file, " R%d", Get_rd());
  // print second operand
  if (am_ty == TY_AM32_R_IMM || am_ty == TY_AM64_R_IMM) {
    fprintf(file, " (%d)", Get_imm());
  } else if (am_ty == TY_AM32_R_OFS || am_ty == TY_AM64_R_OFS) {
    fprintf(file, " (%d)", Get_ofs());
  } else {
    fprintf(file, " R%d", _expand2);
  }
  // print third operand
  if (am_ty == TY_AM32S_R_R_IMM || am_ty == TY_AM64_R_R_IMM) {
    fprintf(file, " (%d)", Get_imm());
  } else if (am_ty == TY_AM32S_R_R_OFS || am_ty == TY_AM64_R_R_OFS) {
    fprintf(file, " (%d)", Get_ofs());
  } else if (am_ty == TY_AM32S_R_R_R || am_ty == TY_AM64_R_R_R) {
    fprintf(file, " R%d", Get_rt());
  }
  fprintf(file, "\n");
}

void U_MODULE::Print_header(FILE *file) const {
  Is_True(_mhdr != NULL, ("Mhdr is null."));
  fprintf(file, DBAR);
  fprintf(file, "FILE HEADER\n");
  fprintf(file, DBAR);
  _mhdr->Print(file);
}

void U_MODULE::Print_sec_header(FILE *file) const {
  Is_True(_mhdr != NULL, ("Mhdr is null."));
  Is_True(_shdr_tab != NULL, ("Section header is null."));
  fprintf(file, DBAR);
  fprintf(file, "SECTION HEADERS\n");
  fprintf(file, DBAR);
  for (UINT32 i = 0; i < _mhdr->Get_shdr_num(); i++) {
    fprintf(file, "[SH] id: %02d, ", i + 1);
    _shdr_tab[i].Print(file);
  }
}

void U_MODULE::Print_one_type(FILE *file, UINT32 idx) const {
  UBH_TYPE &type = Get_type(idx);
  UINT32 ret_num = type.Get_ret_num();
  UINT32 param_num = Get_param_num(idx);
  fprintf(file, "(");
  for (UINT32 i = 0; i < _ret_tab[idx].size(); i++) {
    fprintf(file, "%s", Get_raw_type_name(_ret_tab[idx][i]));
    if (i != _ret_tab[idx].size() - 1) {
      fprintf(file, ", ");
    }
  }
  fprintf(file, ") (");
  for (UINT32 i = 0; i < _param_tab[idx].size(); i++) {
    fprintf(file, "%s p%d", Get_raw_type_name(_param_tab[idx][i]), i + 1);
    if (i != _param_tab[idx].size() - 1) {
      fprintf(file, ", ");
    }
  }
  fprintf(file, ")\n");
}

void U_MODULE::Print_type_cache(FILE *file) const {
  fprintf(file, DBAR
                "TYPE TABLE:\n"
                DBAR);
  if (!Contains_section(S_EXPORT)) {
    fprintf(file, "Empty\n");
    return;
  }
  for (UINT32 i = 0; i < Get_shdr_by_sec_id(S_TYPE).Get_elem_num(); i++) {
    UBH_TYPE &type = Get_type(i);
    fprintf(file, "[%4d] origin: ", i);
    type.Print(file, *this);
    fprintf(file, "[%4d] cache:  ", i);
    Print_one_type(file, i);
  }
}

void U_MODULE::Print_import_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "IMPORT SECTION\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_IMPORT)) {
    fprintf(file, "Empty\n");
    return;
  }
  UBH_SHDR &import_header = Get_shdr_by_sec_id(S_IMPORT);
  for (UINT32 i = 0; i < import_header.Get_elem_num(); i++) {
    UBH_IMPORT &import = Get_import(i);
    import.Print(file);
  }
}

void U_MODULE::Print_imp_nat_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "IMPORT NATIVE SECTION\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_IMP_NAT)) {
    fprintf(file, "Empty\n");
    return;
  }
  UBH_SHDR &imp_nat_shdr = Get_shdr_by_sec_id(S_IMP_NAT);
  for (UINT32 i = 0; i < imp_nat_shdr.Get_elem_num(); i++) {
    UBH_IMP_NAT &imp_nat_entry = Get_imp_nat(i);
    imp_nat_entry.Print(file);
  }
}

void U_MODULE::Print_export_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "EXPORT SECTION\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_EXPORT)) {
    fprintf(file, "Empty\n");
    return;
  }
  UBH_SHDR &export_header = Get_shdr_by_sec_id(S_EXPORT);
  for (UINT32 i = 0; i < export_header.Get_elem_num(); i++) {
    UBH_EXPORT &export_entry = Get_export(i);
    export_entry.Print(file);
  }
}

void U_MODULE::Print_func_header(UINT32 func_idx, FILE *file) const {
  UBH_FUNC &func = Get_func(func_idx);
  fprintf(file, ";; Function index: %d, ", func_idx);
  fprintf(file, "%s ", Get_module_name(func.Get_name_idx()));
  fprintf(file, " [%d] ", func.Get_type_idx());
  Print_one_type(file, func.Get_type_idx());
  // print ret and param number classified by type
  fprintf(file, ";; RET and PARAM num:\t\t(i32: %d, i64: %d, f32: %d, f64: %d) ",
          Get_func_ret_num<I32>(func_idx), Get_func_ret_num<I64>(func_idx),
          Get_func_ret_num<F32>(func_idx), Get_func_ret_num<F64>(func_idx));
  fprintf(file, "(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          Get_func_param_num<I32>(func_idx), Get_func_param_num<I64>(func_idx),
          Get_func_param_num<F32>(func_idx), Get_func_param_num<F64>(func_idx));
}

/*
 * the original print of func just can dump raw data
 * module have full information, can dump detail
 */
void U_MODULE::Print_func_sec(FILE *file) {
  fprintf(file, DBAR
                "FUNCTION TABLE:\n"
                DBAR);
  if (!Contains_section(S_FUNC)) {
    fprintf(file, "No function section found. \n");
    return;
  }
  UINT32 sec_header_idx = Get_shdr_idx(S_FUNC);
  UBH_SHDR &func_header = Get_shdr(sec_header_idx);
  for (UINT32 i = 0; i < func_header.Get_elem_num(); i++) {
    UBH_FUNC &func = Get_func(i);
    Print_func_header(i, file);
    if (func.Is_set_flag(F_EXTERN)) {
      fprintf(file, ";; Extern function\n");
      fprintf(file, SBAR);
      continue;
    }
    UBH_CODE &ubh_code = Get_code(func.Get_code_idx());
    ubh_code.Print(file);
    fprintf(file, SBAR);
  }
}

void U_MODULE::Print_data_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "DATA SECTION\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_DATA)) {
    fprintf(file, "No data section found. \n");
    return;
  }
  UBH_SHDR &shdr = Get_shdr_by_sec_id(S_DATA);
  UINT32 mem_len = shdr.Get_elem_num();
  fprintf(file, "[Memory section] length = %d(0x%08x)\n", mem_len, mem_len);
  char *data_sec = Data_sec();
  for (UINT32 i = 0; i < mem_len; i++) {
    fprintf(file, "%02x", (*(data_sec + i)) & 0xff);
    if ((i % 32) == 31) {
      fprintf(file, "\n");
    }
  }
  fprintf(file, "\n");
}

void U_MODULE::Print_non_null_str_tab(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "NON NULL TERMINATED STRING TABLE\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_NN_STR)) {
    fprintf(file, "No NN_STR-string section found. \n");
    return;
  }
  UBH_SHDR &shdr = Get_shdr_by_sec_id(S_NN_STR);
  const char *nn_str = Non_null_str_sec();
  for (UINT32 i = 0; i < shdr.Get_elem_num(); i++) {
    fprintf(file, "%02x", (*(nn_str + i)) & 0xff);
    if ((i % 32) == 31) {
      fprintf(file, "\n");
    }
  }
  fprintf(file, "\n");
}

void U_MODULE::Print_glbl_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "GLOBAL TABLE\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_GLBL)) {
    fprintf(file, "No global section found. \n");
    return;
  }
  UBH_SHDR &shdr = Get_shdr_by_sec_id(S_GLBL);
  for (UINT32 i = 0; i < shdr.Get_elem_num(); i++) {
    UBH_GLBL &glbl = Get_glbl(i);
    fprintf(file, "[%4d] ", i);
    glbl.Print(file);
  }
}

void U_MODULE::Print_nat_sec(FILE *file) const {
  fprintf(file, DBAR);
  fprintf(file, "NATIVE SECTION\n");
  fprintf(file, DBAR);
  if (!Contains_section(S_NAT)) {
    fprintf(file, "No native section found. \n");
    return;
  }
  UBH_SHDR &shdr = Get_shdr_by_sec_id(S_NAT);
  UBH_SHDR &nat_shdr = Get_shdr_by_sec_id(S_NAT);
  UBH_NAT **nat_sec = Nat_sec();
  char *nat_ptr = nat_sec != nullptr ? (char *) (nat_sec[0]) : nullptr;
  for (UINT32 i = 0; i < shdr.Get_elem_num(); i++) {
    UBH_NAT &bin_entry = Get_nat_entry(i);
    fprintf(file, "[%4u] offset: %4d, size: %u\n", i, (int) (bin_entry.Get_buf() - nat_ptr), bin_entry.Byte_number());
  }
}

void U_MODULE::Print(FILE *file) {
  bool keep_off = Keep_off();
  if (keep_off) {
    Convert_off_to_ptr();
  }
  Print_header(file);
  Print_sec_header(file);
  Print_type_cache(file);
  Print_import_sec(file);
  Print_imp_nat_sec(file);
  Print_export_sec(file);
  Print_func_sec(file);
  Print_data_sec(file);
  Print_glbl_sec(file);
  Print_non_null_str_tab(file);
  Print_nat_sec(file);
  fprintf(file, DBAR);
  fprintf(file, DBAR);
  if (keep_off) {
    Convert_ptr_to_off();
  }
}

UINT8 U_MODULE::Get_sec_id_from_header(UBH_SHDR &sec_header) const {
  UINT8 sec_tag = sec_header.Get_sec_tag();
  if (sec_tag == SEC_TYPE) {
    return S_TYPE;
  } else if (sec_tag == SEC_FUNC) {
    return S_FUNC;
  } else if (sec_tag == SEC_GLBL) {
    return S_GLBL;
  } else if (sec_tag == SEC_TAB) {
    UINT16 kind = sec_header.Get_kind();
    if (kind == K_MODULE_NAME) {
      return S_MODULE_NAME;
    } else if (kind == K_GLBL_NAME) {
      return S_GLBL_NAME;
    } else if (kind == K_NON_NULL_STR) {
      return S_NN_STR;
    } else {
      Is_True(false, ("Wrong SEC_TAB kind, kind: %d.", kind));
    }
  } else if (sec_tag == SEC_MEM) {
    if (sec_header.Get_kind() == MEM_DATA) {
      return S_DATA;
    } else {
      return S_RELOC;
    }
  } else if (sec_tag == SEC_IMPORT) {
    UINT16 kind = sec_header.Get_kind();
    if (kind == K_IMPORT_MODULE) {
      return S_IMPORT;
    } else if (kind == K_IMPORT_NATIVE) {
      return S_IMP_NAT;
    } else {
      Is_True(false, ("Wrong SEC_TAB kind, kind: %d.", kind));
    }
  } else if (sec_tag == SEC_EXPORT) {
    UINT16 kind = sec_header.Get_kind();
    if (kind == K_EXPORT_GLBL) {
      return S_EXPORT_GLBL;
    } else if (kind == K_EXPORT) {
      return S_EXPORT;
    } else {
      Is_True(false, ("Wrong SEC_TAB kind, kind: %d.", kind));
    }
  } else if (sec_tag == SEC_CODE) {
    return S_CODE;
  } else if (sec_tag == SEC_BIN) {
    return S_NAT;
  } else {
    Is_True(false, ("Unsupported section, section tag: %d.", sec_tag));
  }
  // not reachable.
  return S_COUNT;
}

void U_MODULE::Set_shdr_tab(UBH_SHDR *shdr_tab) {
  Is_True(_mhdr != NULL, ("Mhdr is null."));
  _shdr_tab = shdr_tab;
  // cache section id to section header index
  for (UINT32 i = 0; i < _mhdr->Get_shdr_num(); i++) {
    UBH_SHDR &sec_header = _shdr_tab[i];
    if (!sec_header.Have_sec()) {
      continue;
    }
    UINT8 sec_id = Get_sec_id_from_header(sec_header);
    _sec_sec_header_map[sec_id] = i;
  }
}

void U_MODULE::Init_module_from_buffer(U_MODULE *module, char *buffer, UINT32 buffer_len) {
  // Ignoring len for now.
  module->_buffer = buffer;
  module->_buffer_len = buffer_len;
  char *buf_p = buffer;
  module->Set_header((UBH_MHDR *) buf_p);
  if (strncmp(module->Mhdr().Get_umagic(), UBH_MAGIC, UBH_MAGIC_LEN) != 0) {
    fprintf(stderr, "Verify magic failed, wrong ubh file.\n");
    exit(-1);
  }
  buf_p += sizeof(UBH_MHDR);
  module->Set_shdr_tab((UBH_SHDR *) (buffer + module->Mhdr().Get_shdr_off()));
  UINT16 sec_header_num = module->Mhdr().Get_shdr_num();
  for (UINT16 i = 0; i < sec_header_num; i++) {
    UBH_SHDR &sec_header = module->Get_shdr(i);
    if (!sec_header.Have_sec()) {
      continue;
    }
    char *sec_pointer = buffer + sec_header.Get_idx();
    UINT8 sec_id = module->Get_sec_id_from_header(sec_header);
    switch (sec_id) {
      case S_TYPE:
        module->Set_type_sec((UBH_TYPE *) sec_pointer);
        break;
      case S_IMPORT:
        module->Set_import_sec((UBH_IMPORT *) sec_pointer);
        break;
      case S_IMP_NAT:
        module->Set_imp_nat_sec((UBH_IMP_NAT *) sec_pointer);
        break;
      case S_FUNC:
        module->Set_func_sec((UBH_FUNC *) sec_pointer);
        break;
      case S_EXPORT:
        module->Set_export_sec((UBH_EXPORT *) sec_pointer);
        break;
      case S_EXPORT_GLBL:
        module->Set_exp_glbl_sec((UBH_EXP_GLBL *) sec_pointer);
        break;
      case S_DATA:
        module->Set_data_sec(sec_pointer);
        break;
      case S_RELOC:
        module->Set_reloc_sec((UINT32 *) sec_pointer);
        break;
      case S_GLBL:
        module->Set_glbl_sec((UBH_GLBL *) sec_pointer);
        break;
      case S_CODE:
        module->Set_code_sec((char *) sec_pointer);
        break;
      case S_MODULE_NAME:
        module->Set_module_nmae_sec(sec_pointer);
        break;
      case S_GLBL_NAME:
        module->Set_glbl_name_sec(sec_pointer);
        break;
      case S_NN_STR:
        module->Set_non_null_str_sec(sec_pointer);
        break;
      case S_NAT:
        module->Set_nat_sec((UBH_NAT *) sec_pointer);
        break;
      default:
        Is_True(false, ("UNIMPLEMENTED, sec id: %d.", sec_id));
        break;
    }
  }
  // cache type cache immediately
  module->Decode_type_and_cache();
}

void Init_mem_with_data_segs(char *mem_p, UINT32 mem_size,
                             char *data_sec, UINT32 data_sec_size) {
  // Verify
  Is_True(*(UINT64 *)data_sec == *(UINT64 *) DATA_SEGMENT_MAGIC_PREFIX,
          ("Incorrect data sect magic found. %.8s", data_sec));
  UBH_DATA_SEGMENTS &segments = *(UBH_DATA_SEGMENTS *)data_sec;
  Is_True_Rel(segments.Calc_total_space() == data_sec_size,
             ("Total size doesn't match, segs count = %d, "
              "expected size = %u, real total = %u",
              segments.Get_size(), segments.Calc_total_space(), data_sec_size));
  UINT32 max_ofs = segments.Calc_max_ofs();
  if (max_ofs > mem_size) {
    // Incorrect setting, abort.
    Is_True_Rel(false,
                ("Cannot init memory, data section max "
                 "offset+size = %lu(0x%08x) "
                 "limit exceed that of mem_size = %lu(0x%08x)",
                 max_ofs, max_ofs, mem_size, mem_size));
    return;
  }
  UINT32 seg_cnt = segments.Get_size();
  UINT32 total_copied = segments.Calc_size(); // Calculate prepending space.
  for (UINT32 i = 0; i < seg_cnt; i++) {
    Is_True_Rel(segments.Get_seg(i).Get_mem_ofs() + segments.Get_seg(i).Get_size() < mem_size, ("Exceed boundary."));
    memcpy(mem_p + segments.Get_seg(i).Get_mem_ofs(),
           data_sec + total_copied,
           segments.Get_seg(i).Get_size());
    total_copied += segments.Get_seg(i).Get_size(); // Add the copied size to the list.
  }
}

UINT32 U_MODULE::Fix_up_reloc(char *mem_p, UINT32 mem_size) {
  Is_Trace(Tracing(TP_BIN_READ), (TFile, "Fix up relocatable offset:\n"));
  if (!Contains_section(S_DATA)) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "No global section.\n"));
    return NUM_SUCC;
  }
  UBH_SHDR &data_header = Get_shdr_by_sec_id(S_DATA);
  UINT32 data_sec_size = data_header.Get_elem_num();
  if (data_sec_size == 0) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "No global symbol.\n"));
    return NUM_SUCC;
  }
  Is_True(mem_p != NULL, ("Mem pointer can't be NULL, there are global symbols.\n"));
  _real_data_sec = mem_p;
  char *data_sec = Data_sec();
  // Special treatment for magic-wasm data section
  if (memcmp(data_sec, DATA_SEGMENT_MAGIC_PREFIX, 8) != 0) {
    // Just copy
    Is_True_Rel(data_sec_size <= mem_size,
                ("VM init memory is not enough, data section size: %d, memory size: %d.", data_sec_size, mem_size));
    memcpy(_real_data_sec, data_sec, data_sec_size);
  } else {
    // Found magic wasm data section, read and parse it.
    Init_mem_with_data_segs(_real_data_sec, mem_size, data_sec, data_sec_size);
  }
  if (!Contains_section(S_RELOC)) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "No relocation section.\n"));
    return NUM_SUCC;
  }
  UBH_SHDR &reloc_header = Get_shdr_by_sec_id(S_RELOC);
  if (reloc_header.Get_elem_num() == 0) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "No relocation data.\n"));
    return NUM_SUCC;
  }
  UINT32 *reloc_sec = Reloc_sec();
  Is_True(data_sec_size <= mem_size,
          ("VM init memory is not enough, data section size: %d, memory size: %d.", data_sec_size, mem_size));
  if (data_sec_size >= mem_size) {
    return ERR_NOT_ENOUGH_MEM_BUF;
  }
  for (UINT32 i = 0; i < reloc_header.Get_elem_num(); i+=3) {
    UINT32 patch_pos = reloc_sec[i];
    UINT32 ref_idx = reloc_sec[i + 1];
    UINT32 ref_off = reloc_sec[i + 2];
    INTPTR *patch_pos_ptr = (INTPTR *) (_real_data_sec + patch_pos);
    *patch_pos_ptr = (INTPTR) (_real_data_sec + ref_idx + ref_off);
    Is_Trace(Tracing(TP_BIN_READ),
             (TFile, "Data offset:%d, global idx: %d, off: %d, patch addr: %p\n", patch_pos, ref_idx, ref_off, (char *) (*patch_pos_ptr)));
  }
  return NUM_SUCC;
}

// Decode param & ret num in types to avoid future shifting
void U_MODULE::Decode_type_and_cache() {
  UINT32 type_num = Get_shdr_by_sec_id(S_TYPE).Get_elem_num();
  UINT32 func_num = Get_shdr_by_sec_id(S_FUNC).Get_elem_num();

  // initialize
  _param_tab     = new vector<UINT8>[type_num];
  _ret_tab       = new vector<UINT8>[type_num];
  _param_num_tab = new UINT32[type_num * VALTY_COUNT];
  _ret_num_tab   = new UINT32[type_num * VALTY_COUNT];
  memset(_param_num_tab, 0, type_num * VALTY_COUNT * sizeof(UINT32));
  memset(_ret_num_tab, 0, type_num * VALTY_COUNT * sizeof(UINT32));

  _fp_shs_tab          = new UINT64[func_num];
  _reg_num_tab         = new UINT64[func_num];
  memset(_fp_shs_tab, 0, func_num * sizeof(UINT64));
  memset(_reg_num_tab, 0, func_num * sizeof(UINT64));

  UINT8 valty;
  // cache each type
  for (UINT32 i = 0; i < type_num; i++) {
    UBH_TYPE &type = Get_type(i);
    UINT8 param_num = type.Get_param_num();
    UINT8 ret_num = type.Get_ret_num();
    UINT32 type_list = type.Get_elem();
    if (type.Is_stored_local()) {
      // cache ret & param number of each type to {ret,param}_num_tab
      // cache sig to {ret,param}_entry_vec
      for (UINT32 j = 0; j < ret_num; j++) {
        valty = ((type_list >> (MAX_TYPE_ELEM_RANGE - (j + 1) * VALTY_SZ)) & TYPE_MASK);
        _ret_tab[i].push_back(valty);
        _ret_num_tab[i * VALTY_COUNT + valty]++;
      }
      for (UINT32 j = ret_num; j < ret_num + param_num; j++) {
        valty = ((type_list >> (MAX_TYPE_ELEM_RANGE - (j + 1) * VALTY_SZ)) & TYPE_MASK);
        _param_tab[i].push_back(valty);
        _param_num_tab[i * VALTY_COUNT + valty]++;
      }
    } else {
      // ret + param > 13, type info in non null terminated string table
      UINT32 nn_str_idx = type.Get_elem(); // index to non null terminated string table
      UINT8* nn_str_ptr = (UINT8*) Get_non_null_str(nn_str_idx);

      // if param num == 0xff, this number will be placed in nn str
      param_num = *((UINT16*) nn_str_ptr); // 2 byte
      nn_str_ptr = nn_str_ptr + 2;
      // count param number in string table
      UINT8 elem;
      for (UINT32 j = 0; j < ret_num; j++) {
        elem = nn_str_ptr[0];
        valty = (elem >> (CHAR_BIT - (j + 1u) * 2u)) & TYPE_MASK;
        _ret_tab[i].push_back(valty);
        _ret_num_tab[i * VALTY_COUNT + valty]++;
      }
      for (UINT32 j = ret_num; j < ret_num + param_num; j++) {
        elem = nn_str_ptr[j / 4];
        UINT32 off = j % 4;
        valty = (elem >> (CHAR_BIT - (off + 1u) * 2u)) & TYPE_MASK;
        _param_tab[i].push_back(valty);
        _param_num_tab[i * VALTY_COUNT + valty]++;
      }
    }
  }

  for (UINT32 i = 0; i < func_num; i++) {
    UBH_FUNC &func = Get_func(i);
    UINT32 proto_type_idx = func.Get_type_idx();
    if (func.Is_set_flag(F_EXTERN)) {
      continue;
    }
    UBH_CODE &code = Get_code(func.Get_code_idx());
    UINT16 *reg_arr = (UINT16 *) &_reg_num_tab[i];
    reg_arr[0] = code.Get_i32_local() + code.Get_i32_io();
    reg_arr[1] = code.Get_i64_local() + code.Get_i64_io();
    reg_arr[2] = code.Get_f32_local() + code.Get_f32_io();
    reg_arr[3] = code.Get_f64_local() + code.Get_f64_io();
    UINT16 *shs_arr = (UINT16 *) &_fp_shs_tab[i];
    shs_arr[0] = code.Get_i32_local();
    shs_arr[1] = code.Get_i64_local();
    shs_arr[2] = code.Get_f32_local();
    shs_arr[3] = code.Get_f64_local();
  }
}

U_MODULE::U_MODULE() : _sec_sec_header_map() {
  _expanded_table = nullptr;
  _expanded_func_base = nullptr;
  _param_num_tab = nullptr;
  _ret_num_tab = nullptr;
  _param_tab = nullptr;
  _ret_tab = nullptr;
  _lab_tab = nullptr;
  _reg_num_tab = nullptr;
  _fp_shs_tab = nullptr;
  _expanded_pc_map = nullptr;
  _origin_pc_map = nullptr;
  _nat_sec = nullptr;
  _keep_off = false;
}

U_MODULE::~U_MODULE() {
  delete[] _expanded_table;
  delete[] _expanded_func_base;
  delete[] _param_num_tab;
  delete[] _ret_num_tab;
  delete[] _param_tab;
  delete[] _ret_tab;
  delete[] _lab_tab;
  delete[] _reg_num_tab;
  delete[] _fp_shs_tab;
  delete[] _expanded_pc_map;
  delete[] _origin_pc_map;
  delete[] _nat_sec;
}

// Expand code to avoid further bit operations
void U_MODULE::Expand_code() {
  // go through 1
  // calculate decoded tables size
  if (Keep_off()) {
    Convert_off_to_ptr();
  }
  UINT32 code_sz = 0;
  UINT32 func_num = Get_shdr_by_sec_id(S_FUNC).Get_elem_num();
  for (UINT32 i = 0; i < func_num; i++) {
    UBH_FUNC &func = Get_func(i);
    if (func.Is_set_flag(F_EXTERN)) {
      continue;
    }
    UBH_CODE &code = Get_code(func.Get_code_idx());
    code_sz += code.Get_code_size();
  }
  // alloc space
  // add one more break instruction at the end
  _expanded_table     = new AM_EXPAND[code_sz + 2]; // will fix up at the end
  _expanded_func_base = new AM_EXPAND*[func_num];
  _lab_tab            = new AM_EXPAND**[func_num];
  _expanded_pc_map    = new UINT32[code_sz];          // will fix up at the end
  _origin_pc_map      = new UINT32[code_sz];

  // go through 2
  // fill in decoded tables
  UINT32 code_base_idx = 0, lab_num, inst_num = 0;
  UINT32 *code_begin, *lab_tab;
  TY_AM am_ty;
  AM_EXPAND *e_inst_p = _expanded_table;
  for (UINT32 i = 0; i < func_num; i++) {
    UBH_FUNC &func = Get_func(i);
    if (func.Is_set_flag(F_EXTERN)) {
      continue;
    }
    UBH_CODE &code = Get_code(func.Get_code_idx());
    code_begin = code.Get_inst_tab();
    code_sz = code.Get_code_size();
    U_INST_ITER end(code_begin + code_sz);
    for (U_INST_ITER iter(code_begin); iter != end; iter++, inst_num++, e_inst_p++) {
      UINT32 *addr = iter.Addr();
      UINT32 offset = addr - code_begin;
      _origin_pc_map[code_base_idx + offset] = inst_num;
      _expanded_pc_map[inst_num] = code_base_idx + offset; // map expanded pc to origin pc
      Is_Trace_cmd(true, {
        Bind_expand_pc(e_inst_p, i, offset);
      });
      pair<bool, UINT64> inst_pair = *iter;
      if (inst_pair.first) {
        U_INST32 inst(inst_pair.second);
        e_inst_p->Set_opcode(U_INST_UTIL::Opcode_to_opcode_l(inst.Opcode(), inst.Valtype()));
        TY_AM ty_am = inst.Ty_am();
        switch (ty_am) {
          case TY_AM32:
            break;
          case TY_AM32_OFS:
            e_inst_p->Set_ofs(inst.OFS._ofs);
            break;
          case TY_AM32_R:
            e_inst_p->Set_rd(inst.R._r1);
            break;
          case TY_AM32_R_IMM:
            e_inst_p->Set_rd(inst.R_IMM._r1);
            e_inst_p->Set_imm(inst.R_IMM._imm);
            break;
          case TY_AM32_R_OFS:
            e_inst_p->Set_rd(inst.R_OFS._r1);
            e_inst_p->Set_ofs(inst.R_OFS._ofs);
            break;
          case TY_AM32_R_R:
            e_inst_p->Set_rd(inst.R_R._r1);
            e_inst_p->Set_rs(inst.R_R._r2);
            break;
          case TY_AM32S_R_R_IMM:
            e_inst_p->Set_rd(inst.R_R_IMM._r1);
            e_inst_p->Set_rs(inst.R_R_IMM._r2);
            e_inst_p->Set_imm(inst.R_R_IMM._imm);
            break;
          case TY_AM32S_R_R_OFS:
            e_inst_p->Set_rd(inst.R_R_OFS._r1);
            e_inst_p->Set_rs(inst.R_R_OFS._r2);
            e_inst_p->Set_ofs(inst.R_R_OFS._ofs);
            break;
          case TY_AM32S_R_R_R:
            e_inst_p->Set_rd(inst.R_R_R._r1);
            e_inst_p->Set_rs(inst.R_R_R._r2);
            e_inst_p->Set_rt(inst.R_R_R._r3);
            break;
          default:
            Is_True(false, ("Wrong ty_am: %d.", ty_am));
        }
      } else {
        U_INST64 inst(inst_pair.second);
        TY_AM ty_am = inst.Ty_am();
        e_inst_p->Set_opcode(U_INST_UTIL::Opcode_to_opcode_l(inst.Opcode(), inst.Valtype()));
        switch (ty_am) {
          case TY_AM64_R_IMM:
            e_inst_p->Set_rd(inst.R_IMM._r1);
            e_inst_p->Set_imm(inst.R_IMM._imm);
            break;
          case TY_AM64_R_OFS:
            e_inst_p->Set_rd(inst.R_OFS._r1);
            e_inst_p->Set_ofs(inst.R_OFS._ofs);
            break;
          case TY_AM64_R_R_IMM:
            e_inst_p->Set_rd(inst.R_R_IMM._r1);
            e_inst_p->Set_rs(inst.R_R_IMM._r2);
            e_inst_p->Set_imm(inst.R_R_IMM._imm);
            break;
          case TY_AM64_R_R_OFS:
            e_inst_p->Set_rd(inst.R_R_OFS._r1);
            e_inst_p->Set_rs(inst.R_R_OFS._r2);
            e_inst_p->Set_ofs(inst.R_R_OFS._ofs);
            break;
          case TY_AM64_R_R_R:
            e_inst_p->Set_rd(inst.R_R_R._r1);
            e_inst_p->Set_rs(inst.R_R_R._r2);
            e_inst_p->Set_rt(inst.R_R_R._r3);
            break;
          default:
            Is_True(false, ("Wrong ty_am: %d.", ty_am));
        }
      }
    }
    // decode label table
    lab_tab = code.Get_label_tab(); // label table placed right after code
    lab_num = code.Get_label_num();
    _lab_tab[i] = new AM_EXPAND*[lab_num];
    for (UINT32 j = 0; j < lab_num; j++)
      _lab_tab[i][j] = _expanded_table + (_origin_pc_map[code_base_idx + lab_tab[j]]);

    // each function has a start pc
    _expanded_func_base[i] = _expanded_table + _origin_pc_map[code_base_idx];
    code_base_idx += code_sz; // start idx of next code
  }
  // add break instruction
  _end_pc = e_inst_p; e_inst_p++;
  e_inst_p->Set_opcode(BREAK << VALTY_SZ);
}

void  U_MODULE::Set_code_sec(char *code_sec) {
  if (!Contains_section(S_CODE)) {
    Is_Trace(Tracing(TP_BIN_READ), (TFile, "No code section found\n"));
    return;
  }
  UBH_SHDR &code_shdr = Get_shdr_by_sec_id(S_CODE);
  UINT32 code_elem_num = code_shdr.Get_elem_num();
  char *code_sec_p = code_sec;
  _code_sec = new UBH_CODE *[code_elem_num];
  for (UINT32 i = 0; i < code_shdr.Get_elem_num(); i++) {
    UBH_CODE *code = (UBH_CODE *) code_sec_p;
    _code_sec[i] = code;
    if (!Keep_off()) {
      code->Set_inst_tab((UINT32 *) (code_sec + code->Get_inst_lab_off()));
      code->Set_label_tab((UINT32 *) (code_sec + code->Get_label_tab_off()));
    }
    code_sec_p += sizeof(UBH_CODE) + code->Get_code_size() * sizeof(UINT32) + code->Get_label_num() * sizeof(UINT32);
  }
}

void U_MODULE::Set_nat_sec(UBH_NAT *nat_sec) {
  UBH_SHDR &nat_shdr = Get_shdr_by_sec_id(S_NAT);
  _nat_sec = new UBH_NAT *[nat_shdr.Get_elem_num()];
  char *nat_sec_p = (char *) nat_sec;
  for (UINT32 i = 0; i < nat_shdr.Get_elem_num(); i++) {
    UBH_NAT *nat_entry = (UBH_NAT *) nat_sec_p;
    _nat_sec[i] = nat_entry;
    if (!Keep_off()) {
      nat_entry->Set_buf((char *) nat_sec + nat_entry->Buf_offset());
    }
    nat_sec_p += sizeof(UBH_NAT) + nat_entry->Byte_number();
  }
}

void U_MODULE::Convert_off_to_ptr() {
  if (!Keep_off()) {
    return;
  }
  _keep_off = false;
  if (Contains_section(S_CODE)) {
    UBH_SHDR &code_shdr = Get_shdr_by_sec_id(S_CODE);
    UBH_CODE **code_sec = Code_sec();
    char *code_ptr = code_sec != nullptr ? (char *) (code_sec[0]) : nullptr;
    for (UINT32 i = 0; i < code_shdr.Get_elem_num(); i++) {
      UBH_CODE &code = Get_code(i);
      code.Set_inst_tab((UINT32 *) (code_ptr + code.Get_inst_lab_off()));
      code.Set_label_tab((UINT32 *) (code_ptr + code.Get_label_tab_off()));
    }
  }
  if (Contains_section(S_NAT)) {
    UBH_SHDR &bin_shdr = Get_shdr_by_sec_id(S_NAT);
    UBH_NAT **bin_sec = Nat_sec();
    char *bin_ptr = bin_sec != nullptr ? (char *) (bin_sec[0]) : nullptr;
    for (UINT32 i = 0; i < bin_shdr.Get_elem_num(); i++) {
      UBH_NAT &bin_entry = Get_nat_entry(i);
      bin_entry.Set_buf(bin_ptr + bin_entry.Buf_offset());
    }
  }
}

void U_MODULE::Convert_ptr_to_off() {
  if (Keep_off()) {
    return;
  }
  _keep_off = true;
  if (Contains_section(S_CODE)) {
    UBH_SHDR &code_shdr = Get_shdr_by_sec_id(S_CODE);
    UBH_CODE **code_sec = Code_sec();
    char *code_ptr = code_sec != nullptr ? (char *) (code_sec[0]) : nullptr;
    for (UINT32 i = 0; i < code_shdr.Get_elem_num(); i++) {
      UBH_CODE &code = Get_code(i);
      code.Set_inst_tab_off((char *) code.Get_inst_tab() - code_ptr);
      code.Set_label_tab_off((char *) code.Get_label_tab() - code_ptr);
    }
  }
  if (Contains_section(S_NAT)) {
    UBH_SHDR &bin_shdr = Get_shdr_by_sec_id(S_NAT);
    UBH_NAT **bin_sec = Nat_sec();
    char *bin_ptr = bin_sec != nullptr ? (char *) (bin_sec[0]) : nullptr;
    for (UINT32 i = 0; i < bin_shdr.Get_elem_num(); i++) {
      UBH_NAT &bin_entry = Get_nat_entry(i);
      bin_entry.Set_buf_off(bin_entry.Get_buf() - bin_ptr);
    }
  }
}


AM_EXPAND * U_MODULE::Map_to_expand_pc(UINT32 func_id, UINT32 ubh_pc) const {
  UBH_PC_INFO info;
  info._func_id = func_id;
  info._ubh_pc = ubh_pc;
  Is_True_Rel(_pc_to_exp_map.find(info._hashval) != _pc_to_exp_map.end(),
              ("Cannot func_id, ubh_pc in expand mapping."));
  AM_EXPAND *exp = _pc_to_exp_map.at(info._hashval);
  return exp;
}


BOOL  U_MODULE::Map_to_ubh_pc(AM_EXPAND *exp, UINT32 &func_id, UINT32 &ubh_pc) const {
  UBH_PC_INFO info;
  Is_True_Rel(_exp_to_pc_map.find(exp) != _exp_to_pc_map.end(),
              ("Cannot func_id, ubh_pc in expand mapping."));
  info._hashval = _exp_to_pc_map.at(exp);
  func_id = info._func_id;
  ubh_pc = info._ubh_pc;
  return true;
}


void  U_MODULE::Bind_expand_pc(AM_EXPAND *exp, UINT32 func_id, UINT32 ubh_pc){
  UBH_PC_INFO info;
  info._func_id = func_id;
  info._ubh_pc = ubh_pc;
  _exp_to_pc_map.insert(std::make_pair(exp, info._hashval));
}

void U_MODULE::Mprotect_bin_buff() {
  if (!Contains_section(S_NAT)) {
    return;
  }
  uintptr_t pagesize = sysconf(_SC_PAGE_SIZE);
  UBH_SHDR &bin_shdr = Get_shdr_by_sec_id(S_NAT);
  for (UINT32 i = 0; i < bin_shdr.Get_elem_num(); i++) {
    UBH_NAT &bin_entry = Get_nat_entry(i);
    char *buf = bin_entry.Get_buf();
    UINT32 buf_len = bin_entry.Byte_number();
    uintptr_t buf_ptr = (uintptr_t) buf;
    uintptr_t page_start = buf_ptr & -pagesize;
    buf_len += buf_ptr - page_start;
    INT32 ret = mprotect((char *) page_start, buf_len + 1, PROT_READ|PROT_EXEC);
    if (ret != 0) {
      fprintf(stderr, "mprotect binary buffer error, errno: %d.", errno);
      exit(-1);
    }
  }
}

UINT32 U_MODULE::Prepare_before_run(char *mem_p, UINT32 mem_size) {
  UINT32 ret;
  ret = Fix_up_reloc(mem_p, mem_size);
  if (ret != NUM_SUCC) {
    return ret;
  }
  Expand_code();
  Mprotect_bin_buff();
  return NUM_SUCC;
}
