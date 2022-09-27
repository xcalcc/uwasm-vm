/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "ubh/ubh.h"
#include "ubh/u_module.h"
#include "u_inst.h"
#include "utilities/u_str_tab.h"

#define UC_MAG0  'u'
#define   UIDX_MAG0 0
#define UC_MAG1  'A'
#define   UIDX_MAG1 1
#define UC_MAG2  'S'
#define   UIDX_MAG2 2
#define UC_MAG3  'M'
#define   UIDX_MAG3 3

#define UBH_MHDR_SIZE 12
#define UBH_SHDR_SIZE 8
#define UBH_TYPE_SIZE 4
#define UBH_IMPORT_SIZE 4
#define UBH_IMP_NAT_SIZE 12
#define UBH_GLBL_SIZE 16
#define UBH_EXPORT_SIZE 4
#define UBH_EXP_GLBL_SIZE 8
#define UBH_FUNC_SIZE 8
#define UBH_CODE_SIZE 40
#define UBH_NAT_SIZE 16

const char *Get_raw_type_name(UINT8 type) {
  Is_True(type < 4, ("Raw type is not correct, type: %d.", type));
  if (type == 0) {
    return "i32";
  } else if (type == 1) {
    return "i64";
  } else if (type == 2) {
    return "f32";
  } else if (type == 3) {
    return "f64";
  } else {
    // Not reachable
    return "unknown";
  }
}

void UBH_MHDR::Print(FILE *file) const {
  fprintf(file,
          "[HEADER] magic: %c%c%c%c, section header size: %d, section number: %d\n",
          _uMagic[0], _uMagic[1], _uMagic[2], _uMagic[3],
          _shdr_off, _shdr_num);
}

void UBH_SHDR::Print(FILE *file) {
  fprintf(file, "tag: %2d, ", Get_sec_tag());
  UINT8 s = Select_struct();
  if (s == 1) {
    fprintf(file, "        , elem num: %4d, ", U.S1._elem_num);
  } else if (s == 2) {
    fprintf(file, "kind: %2d, elem num: %4d, ",
            U.S2._kind, U.S2._elem_num);
  } else if (s == 3) {
    fprintf(file, "                      , ");
  }
  fprintf(file, "index: %4d\n", _idx);
}

void Print_char_as_binary(FILE *file, char elem) {
  char t = 0x0001;
  for (INT32 i = 0; i < 8; i++) {
    if ((elem & (t << (8 - i - 1))) == 0) {
      fprintf(file, "0");
    } else {
      fprintf(file, "1");
    }
  }
}

void UBH_TYPE::Print(FILE *file, const U_MODULE &module) const {
  UINT32 ret_num = _ret_num;
  UINT32 param_num = _param_num;
  char *nn_str = nullptr;
  if (!Is_stored_local()) {
    nn_str = module.Get_non_null_str(_elem);
    param_num = *((UINT16 *) nn_str);
    nn_str += 2;
  }
  fprintf(file, "ret num: %d, param num: %d, raw data: ",
    ret_num, param_num);
  if (Is_stored_local()) {
    fprintf(file, "0x%08x", _elem);
    fprintf(file, " (");
    UINT32 t = 0x0000000001;
    for (INT32 i = MAX_TYPE_ELEM_RANGE - 1; i >= 0; i--) {
      if ((_elem & (t << i)) == 0) {
        fprintf(file, "0");
      } else {
        fprintf(file, "1");
      }
      if (i != 0 && (i % 8) == 0) {
        fprintf(file, " ");
      }
    }
    fprintf(file, ")\n");
  } else {
    fprintf(file, "nn_str[%d]", _elem);
    UINT32 ret_parm_num = ret_num + param_num;
    fprintf(file, " (");
    UINT32 byte_num = (ret_num + param_num) * 2 / 8;
    if ((ret_num + param_num) * 2 % 8 != 0) {
      byte_num++;
    }
    for (UINT32 j = 0; j < byte_num; j++) {
      Print_char_as_binary(file, *(nn_str + j));
      if (j != byte_num - 1) {
        fprintf(file, " ");
      }
    }
    fprintf(file, ")\n");
  }
}

void UBH_IMPORT::Print(FILE *file) const {
  fprintf(file, "Import ");
  switch (Get_desc_tag()) {
    case K_FUNC:
      fprintf(file, "function: %4u\n", Get_func_idx());
      break;
    default:
      Is_True(false, ("Unsupported desc tag, tag: %u.", Get_desc_tag()));
      break;
  }
}

void UBH_IMP_NAT::Print(FILE *file) const {
  fprintf(file, "Import native ");
  switch (Get_desc_tag()) {
    case K_FUNC:
      fprintf(file, "function: %4u, native type offset: %4u, ", Get_func_idx(), Get_ntype_off());
      if (Get_nat_off() != 0xffffff) {
        fprintf(file, "native section: %4u, native function offset: %4u\n", Get_nat_idx(), Get_nat_off());
      } else {
        fprintf(file, "no in native section.\n");
      }
      break;
    default:
      Is_True(false, ("Unsupported desc tag, tag:%d.", Get_desc_tag()));
      break;
  }
}

void UBH_EXPORT::Print(FILE *file) const {
  fprintf(file, "Export ");
  switch (Get_desc_tag()) {
    case K_FUNC:
      fprintf(file, "function: %d\n", Get_func_idx());
      break;
    default:
      Is_True(false, ("Unsupported desc tag, tag: %d.", Get_desc_tag()));
      break;
  }
}

void UBH_FUNC::Print(FILE *file) const {
  fprintf(file,
          "[FUNC_SEC] type index: %d, name index: %d",
          _type_idx, _name_idx);
  if (_flags != 0) {
    fprintf(file, " ;; Flags: %04x, ", Get_flags());
    if ((_flags & F_EXTERN) != 0) {
      fprintf(file, " extern");
    }
    if (_flags & F_NATIVE) {
      fprintf(file, " native");
    }
  }
  fprintf(file, ";; Code index: %d\n", Get_code_idx());
}

void UBH_GLBL::Print(FILE *file) const {
  fprintf(file, "off: %4d, mut: %4d, type: %s\n", _data_idx, _mut, Get_raw_type_name(_ty));
}

void UBH_CODE::Print(FILE *file) const {
  // print code info
  fprintf(file, ";; Local register num:\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          Get_i32_local(), Get_i64_local(), Get_f32_local(), Get_f64_local());
  fprintf(file, ";; IO register num:\t\t\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          Get_i32_io(), Get_i64_io(), Get_f32_io(), Get_f64_io());
  fprintf(file, ";; All register num:\t\t(i32: %d, i64: %d, f32: %d, f64: %d)\n",
          Get_i32_local() + Get_i32_io(), Get_i64_local() + Get_i64_io(),
          Get_f32_local() + Get_f32_io(), Get_f64_local() + Get_f64_io());
  fprintf(file, ";; Stack size: %d\n", Get_stack_size());
  fprintf(file, ";; Code sz: %d, label number: %d\n", Get_code_size(), Get_label_num());
  // print instruction table
  UINT32 *inst_tab = Get_inst_tab();
  fprintf(file, "\n");
  if (Get_code_size() > 0) {
    U_INST_ITER end(inst_tab + Get_code_size());
    for (U_INST_ITER iter(inst_tab); iter != end; iter++) {
      UINT32 ofst = iter.Addr() - inst_tab;
      fprintf(file, "[%04d]\t\t", ofst);
      pair<bool, UINT64> inst_pair = *iter;
      if (inst_pair.first) {
        U_INST32 inst(inst_pair.second);
        inst.Print(file);
      } else {
        U_INST64 inst(inst_pair.second);
        inst.Print(file);
      }
    }
  } else {
    fprintf(file, "No instruction.\n");
  }
  // print label table
  fprintf(file, "\n[LBL]\n");
  UINT32 *label = Get_label_tab();
  if (Get_label_num() == 0) fprintf(file, "empty table\n");
  for (UINT32 i = 0; i < Get_label_num(); i++)
    fprintf(file, "%04d: %04d\n", i, label[i]);
}

BOOL Check_ubh_ds_size() {
  BOOL pass;
  pass = sizeof(UBH_MHDR) == UBH_MHDR_SIZE;
  Is_True(pass, ("UBH_MHDR size is not correct, size: %d, required: %d.", sizeof(UBH_MHDR), UBH_MHDR_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_SHDR) == UBH_SHDR_SIZE;
  Is_True(pass, ("UBH_SHDR size is not correct, size: %d, required: %d.", sizeof(UBH_SHDR), UBH_SHDR_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_TYPE) == UBH_TYPE_SIZE;
  Is_True(pass, ("UBH_TYPE size is not correct, size: %d, required: %d.", sizeof(UBH_TYPE), UBH_TYPE_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_IMPORT) == UBH_IMPORT_SIZE;
  Is_True(pass, ("UBH_IMPORT size is not correct, size: %d, required: %d.", sizeof(UBH_IMPORT), UBH_IMPORT_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_IMP_NAT) == UBH_IMP_NAT_SIZE;
  Is_True(pass, ("UBH_IMP_NAT size is not correct, size: %d, required: %d.", sizeof(UBH_IMP_NAT), UBH_IMP_NAT_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_GLBL) == UBH_GLBL_SIZE;
  Is_True(pass, ("UBH_GLBL size is not correct, size: %d, required: %d.", sizeof(UBH_GLBL), UBH_GLBL_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_EXPORT) == UBH_EXPORT_SIZE;
  Is_True(pass, ("UBH_EXPORT size is not correct, size: %d, required: %d.", sizeof(UBH_EXPORT), UBH_EXPORT_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_EXP_GLBL) == UBH_EXP_GLBL_SIZE;
  Is_True(pass, ("UBH_EXP_GLBL size is not correct, size: %d, required: %d.", sizeof(UBH_EXP_GLBL), UBH_EXP_GLBL_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_FUNC) == UBH_FUNC_SIZE;
  Is_True(pass, ("UBH_FUNC size is not correct, size: %d, required: %d.", sizeof(UBH_FUNC), UBH_FUNC_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_NAT) == UBH_NAT_SIZE;
  Is_True(pass, ("UBH_NAT size is not correct, size: %d, required: %d.", sizeof(UBH_NAT), UBH_NAT_SIZE));
  if (!pass) {
    return pass;
  }
  pass = sizeof(UBH_CODE) == UBH_CODE_SIZE;
  Is_True(pass, ("UBH_CODE size is not correct, size: %d, required: %d.", sizeof(UBH_CODE), UBH_CODE_SIZE));
  return pass;
}

UBH_SHDR::UBH_SHDR(UINT16 sec_tag, UINT16 elem_num, UINT32 idx) :_idx(idx) {
  // fill 0
  U.S3._sec_tag = 0;
  U.S3._dummy = 0;
  Set_sec_tag(sec_tag);
  Set_elem_num(elem_num);
}

UINT8 UBH_SHDR::Select_struct() const {
  UINT8 sec_tag = Get_sec_tag();
  switch (sec_tag) {
    case SEC_TYPE:
    case SEC_FUNC:
    case SEC_MEM:
    case SEC_CODE:
    case SEC_CUST:
    case SEC_BIN:
      return 1;
    case SEC_IMPORT:
    case SEC_TAB:
    case SEC_GLBL:
    case SEC_EXPORT:
      return 2;
    case SEC_START:
    case SEC_DEBUG:
      return 3;
    default:
      Is_True(false, ("Un-supported sec tag, tag: %d.", sec_tag));
      return 0;
  }
}

UINT32 UBH_SHDR::Get_elem_num() const {
  UINT8 s = Select_struct();
  if (s == 1) {
    return U.S1._elem_num;
  } else if (s == 2) {
    return U.S2._elem_num;
  } else {
    Is_True(false, ("Wrong sec tag, tag: %d.", Get_sec_tag()));
    return 0;
  }
}

UINT16 UBH_SHDR::Get_kind() const {
  UINT8 s = Select_struct();
  if (s == 1) {
    return U.S1._kind;
  } else if (s == 2) {
    return U.S2._kind;
  } else {
    Is_True(false, ("Wrong sec tag, tag: %d.", Get_sec_tag()));
    return 0;
  }
}

void UBH_SHDR::Set_elem_num(UINT16 elem_num) {
  UINT8 s = Select_struct();
  if (s == 1) {
    U.S1._elem_num = elem_num;
  } else if (s == 2) {
    U.S2._elem_num = elem_num;
  }
}

void UBH_SHDR::Set_kind(UINT16 kind) {
  UINT8 s = Select_struct();
  if (s == 1) {
    U.S1._kind = kind;
  } else if (s == 2) {
    U.S2._kind = kind;
  } else {
    Is_True(false, ("Wrong sec tag, tag: %d.", Get_sec_tag()));
  }
}

bool UBH_SHDR::Have_sec() const {
  UINT8 sec_tag = Get_sec_tag();
  if (sec_tag == SEC_START) {
    return false;
  }
  return true;
}

bool UBH_TYPE::operator==(UBH_TYPE type) const {
  return (Get_ret_num() == type.Get_ret_num()) &&
         (Get_param_num() == type.Get_param_num()) &&
         (Get_elem() == type.Get_elem());
}

UINT32 UBH_FUNC::Get_code_idx() const {
  Is_True(!Is_set_flag(F_EXTERN), ("Extern func."));
  return _code_idx;
}
