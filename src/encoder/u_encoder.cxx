/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <cstdint>
#include <vector>
#include "ubh/ubh.h"
#include "ubh/u_module.h"
#include "encoder/u_encoder.h"
#include "utilities/u_utils.h"
#include "ubh/u_inst.h"
#include "u_type_encoder.h"
#include "u_inst_encoder.h"
#include "utilities/u_str_tab.h"

#define START_FUNC_NAME "main"
#define STR_TAB_ALIGN 16

using namespace std;

U_CODE_BODY &U_ENCODER::Cur_code_body() {
  return U_inst_encoder().Cur_code_body();
}

bool U_ENCODER::Pre_check() {
  bool pass = Check_ubh_ds_size();
  if (!pass) {
    return false;
  }
  pass = sizeof(U_INST32) == U_INST32_SIZE;
  Is_True(pass, ("U_INST32 size is not correct, size: %d, required: %d.", sizeof(U_INST32), U_INST32_SIZE));
  if (!pass) {
    return false;
  }
  pass = sizeof(U_INST64) == U_INST64_SIZE;
  Is_True(pass, ("U_INST64 size is not correct, size: %d, required: %d.", sizeof(U_INST64), U_INST64_SIZE));
  if (!pass) {
    return false;
  }
  return true;
}

U_ENCODER::U_ENCODER() : _cur_func_idx(-1), _import_tab(), _export_tab(), _mem_tab(), _func_tab(),
                         _glbl_tab(), _const_data(NULL), _const_data_size(0), _reloc_data_size(0),
                         _module_string_tab() {
  _u_inst_encoder = new U_INST_ENCODER();
  _func_tab.reserve(1024);
  char buf[1];
  buf[0] = '\0';
  _module_string_tab = new U_STR_TAB();
  _module_string_tab->Add(buf, 1);
  _global_name_tab = new U_STR_TAB();
  _global_name_tab->Add(buf, 1);
  _nn_tab = new U_STR_TAB();
  _nn_tab->Add(buf, 1);
  _u_type_encoder = new U_TYPE_ENCODER(*_nn_tab);
  bool check_result = Pre_check();
  if (!check_result) {
    exit(-1);
  }
}

U_ENCODER::~U_ENCODER() {
  delete _u_inst_encoder;
  delete _module_string_tab;
  delete _global_name_tab;
  delete _nn_tab;
  delete _u_type_encoder;
}

UINT32 U_TYPE_ENCODER::Type_to_number(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list) {
  UINT16 ret_num = ret_type_list.size();
  UINT16 param_num = param_type_list.size();
  UINT32 elem = 0;
  UINT32 tmp, i;
  for (i = 0; i < ret_num; i++) {
    tmp = (((UINT8) ret_type_list[i]) & TYPE_MASK); // keep 2 bits
    elem |= (tmp << (VALTY_SZ * (MAX_TYPE_ELEM_NUM - VALTY_SZ - i))); // add 2 bits to elem
  }
  for (i = 0; i < param_num; i++) {
    tmp = (param_type_list[i] & TYPE_MASK);
    elem |= (tmp << (VALTY_SZ * (MAX_TYPE_ELEM_NUM - VALTY_SZ - i - ret_num)));
  }
  return elem;
}

void  U_ENCODER::Set_data_section(UINT64 data_size, char *data_value) {
  Is_True(data_size < UINT32_MAX, ("Cannot alloc memory greater than UINT32_MAX"));
  _const_data = static_cast<UINT8 *>(malloc(data_size));
  Is_True(_const_data != NULL, ("Cannot malloc memory for data copy."));
  _const_data_size = data_size;
  memcpy(_const_data, data_value, data_size);
}

void U_ENCODER::Set_reloc_section(UINT64 reloc_size, UINT32 *reloc_value) {
  _reloc_data = (UINT32 *) malloc(reloc_size * sizeof(UINT32));
  _reloc_data_size = reloc_size;
  memcpy(_reloc_data, reloc_value, reloc_size * sizeof(UINT32));
}

// Add entry to the global table.
UINT32 U_ENCODER::Add_global(const char *sym_name) {
  UINT32 glbl_idx = Global_name_tab().Request(sym_name);
  Is_Trace(Tracing(TP_BIN_WRITE), (TFile, "Add global, global name: %s, offset: %d.\n", sym_name, Global_name_tab().Get_off(glbl_idx)));
  return glbl_idx;
}

/*
 * Declare global variable of given sym_name
 * may be mutable
 * it may has export name 'sym_name'
 * Returns its index in global table.
 */
UINT32 U_ENCODER::Add_global(BOOL mut, UINT32 valty, UINT64 uint64_val, const char *sym_name) {
  Is_True(sym_name != NULL, ("Symbol name cannot be NULL."));
  Is_True(!Global_name_tab().Contains(sym_name), ("Conflict global, name: %s.", sym_name));
  UINT32 glbl_name_idx = Global_name_tab().Add(sym_name);
  Is_Trace(Tracing(TP_BIN_WRITE), (TFile, "Add global, global name: %s, offset: %d, init_val = %llu.\n",
    sym_name, Global_name_tab().Get_off(glbl_name_idx), uint64_val));
  _glbl_tab.emplace_back();
  UBH_GLBL &glbl = _glbl_tab.back();
  // boundary check intended in vector.at().
  glbl.Set_data_idx(0);
  glbl.Set_init_val(uint64_val);
  glbl.Set_ty(valty);
  glbl.Set_mutable(mut);
  return _glbl_tab.size() - 1;
}

/*
 * Declare global variable of given sym_name
 * may be mutable
 * it may has export name 'sym_name'
 * Returns its index in global table.
 */
UINT32 U_ENCODER::Add_global(BOOL mut, UINT32 uint32_val, const char *sym_name) {
  Is_True(sym_name != NULL, ("Symbol name cannot be NULL."));
  Is_True(!Global_name_tab().Contains(sym_name), ("Conflict global, name: %s.", sym_name));
  UINT32 glbl_name_idx = Global_name_tab().Add(sym_name);
  Is_Trace(Tracing(TP_BIN_WRITE), (TFile, "Add global, global name: %s, offset: %d.\n",
    sym_name, Global_name_tab().Get_off(glbl_name_idx)));
  _glbl_tab.emplace_back();
  UBH_GLBL &glbl = _glbl_tab.back();
  // boundary check intended in vector.at().
  glbl.Set_data_idx(uint32_val);
  glbl.Set_init_val(uint32_val);
  glbl.Set_mutable(mut);
  return _glbl_tab.size() - 1;
}

/*
 * Defines a function with given name
 * type index is the index to the prototype table
 * locals define the max size of each VALTYPE register
 * (pair of VALTYPE and number of locals in that valtype).
 * it may have export name
 * Returns function index
 */
UINT32 U_ENCODER::Start_func(UINT32 func_idx, UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size) {
  _cur_func_idx = (INT32) func_idx;
  UINT32 code_idx = U_inst_encoder().Allocate_code_body(local_regs, io_regs, stack_size);
  Func_tab().at(func_idx).Set_code_idx(code_idx);
  return code_idx;
}

void U_ENCODER::Add_to_import_tab(UINT32 func_idx) {
  _import_tab.emplace_back(K_FUNC, func_idx);
}

void U_ENCODER::Add_to_export_tab(UINT32 func_idx) {
  _export_tab.emplace_back(K_FUNC, func_idx);
}

UINT32 U_ENCODER::Inner_request_func(const char *func_name, UINT32 type_idx, UINT32 flags, bool &exist) {
  if (Module_str_tab().Contains(func_name)) {
    exist = true;
    UINT32 func_name_idx = Module_str_tab().Get_idx(func_name);
    for (UINT32 i = 0; i < Func_tab().size(); i++) {
      if (Func_tab()[i].Get_name_idx() == func_name_idx) {
        return i;
      }
    }
    Is_True(false, ("Can't find func entry, func name: %s, func name idx: %d.", func_name, func_name_idx));
    return -1;
  } else {
    exist = false;
    UINT32 entry_idx = Module_str_tab().Add(func_name);
    UINT32 name_idx = Module_str_tab().Get_off(entry_idx);
    Func_tab().emplace_back(type_idx, name_idx, flags);
    return Func_tab().size() - 1;
  }
}

/*
 * Declare an external function
 * type index is the index to the prototype table
 * it may have export name
 * Returns function index
 */
UINT32 U_ENCODER::Request_func(const char *func_name, UINT32 type_idx, UINT32 flags) {
  bool exist;
  UINT32 func_idx = Inner_request_func(func_name, type_idx, flags, exist);
  Is_True((flags & F_NATIVE) == 0, ("Native flag set, please call Request_native_func instead."));
  if (!exist) {
    if ((flags & F_EXTERN) != 0) {
      Add_to_import_tab(Func_tab().size() - 1);
    }
    if ((flags & F_EXPORT) != 0) {
      Add_to_export_tab(Func_tab().size() - 1);
    }
  }
  return func_idx;
}

UINT32 U_ENCODER::Encode_and_save_ntype(NTYPE ret_ntype, vector<NTYPE> &param_ntype_vec) {
  Is_True(ret_ntype < NTYPE_NUM, ("Return native type error, native type id: %d.", ret_ntype));
  UINT32 ntype_num = param_ntype_vec.size();
  UINT32 index = 0;
  UINT8 *ntype_arr;
  if (ret_ntype != NTYPE_V) {
    ntype_num++;
    index++;
    ntype_arr = new UINT8[ntype_num];
    ntype_arr[0] = ret_ntype;
  } else {
    ntype_arr = new UINT8[ntype_num];
  }
  for (UINT32 i = 0; index < ntype_num; i++, index++) {
    ntype_arr[index] = param_ntype_vec[i];
  }
  UINT32 nn_entry_idx = Nn_tab().Request((const char *) ntype_arr, ntype_num);
  delete ntype_arr;
  return Nn_tab().Get_off(nn_entry_idx);
}

void U_ENCODER::Add_to_import_native_tab(UINT32 func_idx, UINT32 ntype_off) {
  Imp_nat_tab().emplace_back(K_FUNC, func_idx, ntype_off);
}

UINT32 U_ENCODER::Request_native_func(const char *func_name, UINT32 type_idx, NTYPE ret_ntype, vector<NTYPE> &param_ntype_vec) {
  bool exist;
  UINT32 func_idx = Inner_request_func(func_name, type_idx, F_EXTERN|F_NATIVE, exist);
  if (!exist) {
    UINT32 ntype_off = Encode_and_save_ntype(ret_ntype, param_ntype_vec);
    Add_to_import_native_tab(Func_tab().size() - 1, ntype_off);
  }
  return func_idx;
}

UINT32 U_ENCODER::Add_inst(UINT32 opcode, VALTYPE valtype, TY_AM ty_am, vector<UINT32> &content) {
  U_CODE_BODY &code_body = Cur_code_body();
  bool is_inst32 = U_INST_UTIL::Is_32bit(ty_am);
  if (is_inst32) {
    UINT32 inst = 0;
    U_INST_UTIL::Init_inst32(&inst, ty_am, opcode, valtype, content);
    code_body.Add_inst32(inst);
  } else {
    UINT64 inst = 0;
    U_INST_UTIL::Init_inst64(&inst, ty_am, opcode, valtype, content);
    code_body.Add_inst64(inst);
  }
  return 0;
}

/*
 * Search for the type ID of given return type & parameter type vector
 * if not found, create a new one
 * return the prototype ID to the caller.
 */
UINT32 U_ENCODER::Add_type(vector<VALTYPE> &ret_type_list, vector<VALTYPE> &param_type_list) {
  UINT32 elem_num = (UINT32) (ret_type_list.size() + param_type_list.size());
  Is_True(elem_num < UINT8_MAX, ("ret + param number out of range: %d >= %d", elem_num, UINT8_MAX));
  for (UINT32 i = 0; i < ret_type_list.size(); i++) {
    Is_True(((UINT32) ret_type_list[i]) < 4, ("Return type is not correct, type value: %d, index: %d.", ret_type_list[i], i));
  }
  for (UINT32 i = 0; i < param_type_list.size(); i++) {
    Is_True(((UINT32)param_type_list.at(i)) < 4, ("Parameter type is not correct, type value: %d, index: %d.", param_type_list[i], i));
  }
  return U_type_encoder().Add_type(ret_type_list, param_type_list);
}

UINT32 U_ENCODER::Request_label(const char *label_name) {
  U_CODE_BODY &code_body = Cur_code_body();
  return code_body.Request_label(label_name);
}

UINT32 U_ENCODER::Add_label(char *label_name) {
  U_CODE_BODY &code_body = Cur_code_body();
  // find label, if not found create a new one
  UINT32 label_idx = code_body.Request_label(label_name);
  // set label to last inst
  code_body.Set_label(label_idx);
  return label_idx;
}

struct SEC_INFO {
  UINT32 _sec_id;
  UINT32 _shdr_tag;
  INT32  _sec_kind;
  UINT32 _elem_size;
};

// each entry is a pair of <SEC, SEC_TAG, ELEM_SIZE>
static SEC_INFO SEC_INFO_ARR[] = {
  // sec_id         shdr_tag       sed_kind         elem_size
  {S_TYPE,          SEC_TYPE,      -1,              sizeof(UBH_TYPE)       },
  {S_EXPORT,        SEC_EXPORT,    K_EXPORT,        sizeof(UBH_EXPORT)     },
  {S_IMPORT,        SEC_IMPORT,    K_IMPORT_MODULE, sizeof(UBH_IMPORT)     },
  {S_IMP_NAT,       SEC_IMPORT,    K_IMPORT_NATIVE, sizeof(UBH_IMP_NAT)    },
  {S_FUNC,          SEC_FUNC,      -1,              sizeof(UBH_FUNC)       },
  {S_CODE,          SEC_CODE,      -1,              0                      },      // code section size should call api
  {S_DATA,          SEC_MEM,       MEM_DATA,        sizeof(UINT8)          },
  {S_GLBL,          SEC_GLBL,      -1,              sizeof(UBH_GLBL)       },
//  {S_EXPORT_GLBL,   SEC_EXPORT,    K_EXPORT_GLBL,   0                      },
  {S_NN_STR,        SEC_TAB,       K_NON_NULL_STR,  1                      },
  {S_MODULE_NAME,   SEC_TAB,       K_MODULE_NAME,   1                      },
  {S_GLBL_NAME,     SEC_TAB,       K_GLBL_NAME,     1                      },
  {S_RELOC,         SEC_MEM,       MEM_RELOC,       sizeof(UINT32)         },
};

void U_ENCODER::Finish() {
  U_inst_encoder().Finish();
  Module_str_tab().Pad(STR_TAB_ALIGN);
  Global_name_tab().Pad(STR_TAB_ALIGN);
  Nn_tab().Pad(STR_TAB_ALIGN);
}

/*
 * calculate total_size, and create ubh_header and collect section id
 */
void U_ENCODER::Collect_write_info(vector<pair<UINT32, UINT32> > &sec_id_vec, vector<UBH_SHDR> &ubh_shdr_vec, UINT32 *total_size) {
  UINT32 size = sizeof(UBH_MHDR);
  UINT32 elem_num = 0;
  for (UINT32 i = 0; i < sizeof(SEC_INFO_ARR) / sizeof(SEC_INFO); i++) {
    SEC_INFO &sec_info = SEC_INFO_ARR[i];
    UINT32 sec_id = sec_info._sec_id;
    switch (sec_id) {
      case S_TYPE:
        elem_num = U_type_encoder().Get_type_list().size();
        break;
      case S_IMPORT:
        elem_num = Import_tab().size();
        break;
      case S_IMP_NAT:
        elem_num = Imp_nat_tab().size();
        break;
      case S_FUNC:
        elem_num = Func_tab().size();
        break;
      case S_DATA:
        elem_num = Const_data_size();
        break;
      case S_GLBL:
        elem_num = Glbl_tab().size();
        break;
      case S_EXPORT:
        elem_num = Export_tab().size();
        break;
      case S_CODE:
        elem_num = U_inst_encoder().U_code_vec().size();
        break;
      case S_NN_STR:
        elem_num = Nn_tab().Get_raw_buf().size();
        break;
      case S_MODULE_NAME:
        elem_num = Module_str_tab().Get_raw_buf().size();
        break;
      case S_GLBL_NAME:
        elem_num = Global_name_tab().Get_raw_buf().size();
        break;
      case S_RELOC:
        elem_num = Reloc_data_size();
        break;
      default:
        Is_True(false, ("Unsupported section id, section id: %d.", sec_id));
        break;
    }
    if (elem_num != 0) {
      ubh_shdr_vec.emplace_back(sec_info._shdr_tag, elem_num, size);
      UBH_SHDR &shdr = ubh_shdr_vec.back();
      if (sec_info._sec_kind != -1) {
        shdr.Set_kind(sec_info._sec_kind);
      }
      // the size of code sec should get from encoder
      UINT32 sec_size;
      if (sec_id != S_CODE) {
        sec_size = elem_num * sec_info._elem_size;
      } else {
        sec_size = U_inst_encoder().Code_sec_size();
      }
      sec_id_vec.push_back(make_pair(sec_id, sec_size));
      size += sec_size;
    }
  }
  for (UINT32 i = 0; i < Func_tab().size(); i++) {
    UBH_FUNC &func = Func_tab()[i];
    char *func_name = &Module_str_tab().Get_raw_buf().at(func.Get_name_idx());
    if (strcmp(func_name, START_FUNC_NAME) == 0) {
      ubh_shdr_vec.emplace_back(SEC_START, 0, i);
      break;
    }
  }
  size += sizeof(UBH_SHDR) * ubh_shdr_vec.size();
  *total_size = size;
}

void U_ENCODER::Write_mhdr(char *buf, UINT32 shdr_num, UINT32 shdr_off) {
  UBH_MHDR *header = (UBH_MHDR *) buf;
  char other_info[UBH_OTHER_LEN] = {0};
  header->Set_magic(UBH_MAGIC);
  header->Set_other(other_info);
  header->Set_shdr_num(shdr_num);
  header->Set_shdr_off(shdr_off);
}

void U_ENCODER::Write_section(char *buf, vector<pair<UINT32, UINT32> > &sec_id_vec) {
  for (UINT32 i = 0; i < sec_id_vec.size(); i++) {
    UINT32 sec_id = sec_id_vec[i].first;
    UINT32 sec_size = sec_id_vec[i].second;
    switch (sec_id) {
      case S_TYPE:
        memcpy(buf, U_type_encoder().Get_type_list().data(), sec_size);
        break;
      case S_IMPORT:
        memcpy(buf, &Import_tab()[0], sec_size);
        break;
      case S_IMP_NAT:
        memcpy(buf, &Imp_nat_tab()[0], sec_size);
        break;
      case S_EXPORT:
        memcpy(buf, &Export_tab()[0], sec_size);
        break;
      case S_FUNC:
        memcpy(buf, &Func_tab()[0], sec_size);
        break;
      case S_DATA:
        memcpy(buf, Const_data(), sec_size);
        break;
      case S_RELOC:
        memcpy(buf, Reloc_data(), sec_size);
        break;
      case S_GLBL:
        memcpy(buf, &Glbl_tab()[0], sec_size);
        break;
      case S_MODULE_NAME:
        memcpy(buf, &Module_str_tab().Get_raw_buf()[0], sec_size);
        break;
      case S_GLBL_NAME:
        memcpy(buf, &Global_name_tab().Get_raw_buf()[0], sec_size);
        break;
      case S_NN_STR:
        memcpy(buf, &Nn_tab().Get_raw_buf()[0], sec_size);
        break;
      case S_CODE:
        U_inst_encoder().Write_to_buf(buf);
        break;
      default:
        Is_True(false, ("Unsupported section, section id: %d.", sec_id));
        break;
    }
    buf += sec_size;
  }
}

void U_ENCODER::Write_shdr(char *buf, vector<UBH_SHDR> &ubh_shdr_vec) {
  memcpy(buf, &ubh_shdr_vec[0], sizeof(UBH_SHDR) * ubh_shdr_vec.size());
}

void U_ENCODER::Write_to_file(const char *file_path) {
  // Dump to internal buffers.
  UINT32 total_size;
  // <sec id, sec size>
  vector<pair<UINT32, UINT32> > sec_id_vec;
  vector<UBH_SHDR> ubh_shdr_vec;
  // should call finish before write, part of section need to fixup some fields
  Finish();
  Collect_write_info(sec_id_vec, ubh_shdr_vec, &total_size);
  Is_Trace(Tracing(TP_BIN_READ), (TFile, "Prepare to write : %d\n", total_size));
  char *buf;
  INT32 fd = Write_and_mmap(file_path, total_size, &buf);
  char *buf_p = buf;
  UINT32 ubh_shdr_off = total_size - ubh_shdr_vec.size() * sizeof(UBH_SHDR);
  Write_mhdr(buf_p, ubh_shdr_vec.size(), ubh_shdr_off);
  buf_p += sizeof(UBH_MHDR);
  Write_section(buf_p, sec_id_vec);
  buf_p = buf + ubh_shdr_off;
  Write_shdr(buf_p, ubh_shdr_vec);
  char *buf_cp = NULL;
  Is_Trace_cmd(Tracing(TP_BIN_WRITE), {
    buf_cp = (char *) malloc(total_size);
    memcpy(buf_cp, buf, total_size);
  });
  Finish_write(fd, buf, total_size);
  Is_Trace_cmd(Tracing(TP_BIN_WRITE), {
    U_MODULE module;
    U_MODULE::Init_module_from_buffer(&module, buf_cp, total_size);
    // ATTENTION: writing action will fixup part of fields such as instruction table offset, label table offset and so on
    // so the buffer can't be reused for another module
    module.Print(TFile);
  });
}

U_CODE_BODY &U_ENCODER::Get_inst_encoder(UINT32 func_idx) {
  // Only used in wasm to uwasm
  UINT32 num_import_funcs = 0;
  for (UINT32 i = 0; i < _func_tab.size(); i++) {
    if (_func_tab.at(i).Get_flags() & U_FUNC_FLAG::F_NATIVE ||
        _func_tab.at(i).Get_flags() & U_FUNC_FLAG::F_IMPORT_WASM) {
      num_import_funcs ++;
    } else {
      break;
    }
  }
  Is_True(func_idx >= num_import_funcs,
          ("Invalid func idx = %u, import func num = %u,"
           "import func doesn't have a code section entry.",
            func_idx, num_import_funcs));

  Is_True(_u_inst_encoder->U_code_vec().size() > (func_idx - num_import_funcs),
          ("Invalid func idx = %u, import func num = %u, code_table_size = %d"
           "import func doesn't have a code section entry.",
    func_idx, num_import_funcs, _u_inst_encoder->U_code_vec().size()));
  return _u_inst_encoder->U_code_vec().at(func_idx - num_import_funcs);
}

void UVM_set_trace(int on) {
  if (on > 0) {
    Set_tracing_opt(TP_BIN_READ, TRACE_ENABLE_LEVEL);
    Set_tracing_opt(TP_BIN_WRITE, TRACE_ENABLE_LEVEL);
    Set_tracing_opt(TP_EXEC, TRACE_ENABLE_LEVEL);
    Set_tracing_opt(TP_EPERF, TRACE_ENABLE_LEVEL);
  }
}
