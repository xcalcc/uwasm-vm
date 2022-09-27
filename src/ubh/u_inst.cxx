/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "u_inst.h"

U_SPECIAL_FORMAT SPECIAL_FORMAT_ARR[] = {
#define U_DIFF_TYPE(opcode, t1, t2, t3) {opcode, t1, t2, t3},
#include "u_diff_type.def"
#undef U_DIFF_TYPE
};

UINT32 SPECIAL_FORMAT_ARR_LEN = sizeof(SPECIAL_FORMAT_ARR) / sizeof(U_SPECIAL_FORMAT);

void U_INST32::Print(FILE* file) const {
  fprintf(file, "%-10s", UWINST_NAME[Opcode()]);
  TY_AM ty_am = Ty_am();
  INT32 s_idx = U_INST_UTIL::Get_special_format((U_OPCODE_LONG) U_INST_UTIL::Opcode_to_opcode_l(Opcode(), Valtype()));
  fprintf(file, " ");
  switch (ty_am) {
    case TY_AM32:
      break;
    case TY_AM32_R: {
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R._r1, file);
      break;
    }
    case TY_AM32_OFS:
      fprintf(file, "%d", OFS._ofs);
      break;
    case TY_AM32_R_IMM:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_IMM._r1, file);
      fprintf(file, " %d", R_IMM._imm);
      break;
    case TY_AM32_R_OFS:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_OFS._r1, file);
      fprintf(file, " %d", R_OFS._ofs);
      break;
    case TY_AM32_R_R:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R._r2, file);
      break;
    case TY_AM32S_R_R_IMM:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_IMM._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_IMM._r2, file);
      fprintf(file, " %d", R_R_IMM._imm);
      break;
    case TY_AM32S_R_R_OFS:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_OFS._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_OFS._r2, file);
      fprintf(file, " %d", R_R_OFS._ofs);
      break;
    case TY_AM32S_R_R_R:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_R._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_R._r2, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 2, Valtype(), R_R_R._r3, file);
      break;
    default:
      Is_True(false, ("Wrong ty_am: %d.", ty_am));
  }
  fprintf(file, "\n");
}

void U_INST64::Print(FILE* file) const {
  fprintf(file, "%-10s", UWINST_NAME[Opcode()]);
  TY_AM ty_am = Ty_am();
  INT32 s_idx = U_INST_UTIL::Get_special_format((U_OPCODE_LONG) U_INST_UTIL::Opcode_to_opcode_l(Opcode(), Valtype()));
  fprintf(file, " ");
  switch (ty_am) {
    case TY_AM64_R_IMM:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_IMM._r1, file);
      fprintf(file, " %d", R_IMM._imm);
      break;
    case TY_AM64_R_OFS:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_OFS._r1, file);
      fprintf(file, " %d", R_OFS._ofs);
      break;
    case TY_AM64_R_R_IMM:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_IMM._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_IMM._r2, file);
      fprintf(file, " %d", R_R_IMM._imm);
      break;
    case TY_AM64_R_R_OFS:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_OFS._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_OFS._r2, file);
      fprintf(file, " %d", R_R_OFS._ofs);
      break;
    case TY_AM64_R_R_R:
      U_INST_UTIL::Print_reg(s_idx, 0, Valtype(), R_R_R._r1, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_R._r2, file);
      fprintf(file, " ");
      U_INST_UTIL::Print_reg(s_idx, 1, Valtype(), R_R_R._r3, file);
      break;
    default:
      Is_True(false, ("Wrong ty_am: %d.", ty_am));
  }
  fprintf(file, "\n");
}

void U_INST_UTIL::Init_inst32(UINT32 *inst, TY_AM ty_am, UINT32 opcode, VALTYPE valtype,
                                     vector<UINT32> &content) {
  U_INST32 *u_inst = (U_INST32 *) inst;
  u_inst->_opcode = opcode;
  u_inst->_ty = valtype;
  UINT32 content_size = content.size();
  switch (ty_am) {
    case TY_AM32:
      Check_content_size(0, content_size);
      break;
    case TY_AM32_OFS:
      Check_content_size(1, content_size);
      Check_imm(22, content.at(0));
      u_inst->OFS._ofs = content.at(0);
      break;
    case TY_AM32_R:
      Check_content_size(1, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R._r1 = content.at(0);
      break;
    case TY_AM32_R_IMM:
      Check_content_size(2, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_IMM._r1 = content.at(0);
      Check_imm(22, content.at(1));
      u_inst->R_IMM._imm = (INT32) content.at(1);
      break;
    case TY_AM32_R_OFS:
      Check_content_size(2, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_OFS._r1 = content.at(0);
      Check_imm(22, content.at(1));
      u_inst->R_OFS._ofs = content.at(1);
      break;
    case TY_AM32_R_R:
      Check_content_size(2, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_R._r1 = content.at(0);
      Check_reg(_10BIT_REG_MAX, content.at(1));
      u_inst->R_R._r2 = content.at(1);
      break;
    case TY_AM32S_R_R_IMM:
      Check_content_size(3, content_size);
      Check_reg(_7BIT_REG_MAX, content.at(0));
      u_inst->R_R_IMM._r1 = content.at(0);
      Check_reg(_7BIT_REG_MAX, content.at(1));
      u_inst->R_R_IMM._r2 = content.at(1);
      Check_imm(8, content.at(2));
      u_inst->R_R_IMM._imm = (INT32) content.at(2);
      break;
    case TY_AM32S_R_R_OFS:
      Check_content_size(3, content_size);
      Check_reg(_7BIT_REG_MAX, content.at(0));
      u_inst->R_R_OFS._r1 = content.at(0);
      Check_reg(_7BIT_REG_MAX, content.at(1));
      u_inst->R_R_OFS._r2 = content.at(1);
      Check_imm(8, content.at(2));
      u_inst->R_R_OFS._ofs = content.at(2);
      break;
    case TY_AM32S_R_R_R:
      Check_content_size(3, content_size);
      Check_reg(_7BIT_REG_MAX, content.at(0));
      u_inst->R_R_R._r1 = content.at(0);
      Check_reg(_7BIT_REG_MAX, content.at(1));
      u_inst->R_R_R._r2 = content.at(1);
      Check_reg(_7BIT_REG_MAX, content.at(2));
      u_inst->R_R_R._r3 = content.at(2);
      break;
    default:
      Is_True(false, ("Wrong ty am: %d.", ty_am));
      break;
  }
}

void U_INST_UTIL::Init_inst64(UINT64 *inst, TY_AM ty_am, UINT32 opcode, VALTYPE valtype,
                              vector<UINT32> &content) {
  U_INST64 *u_inst = (U_INST64 *) inst;
  u_inst->_opcode = opcode;
  u_inst->_ty = valtype;
  UINT32 content_size = content.size();
  switch (ty_am) {
    case TY_AM64_R_IMM:
      Check_content_size(2, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_IMM._r1 = content.at(0);
      Check_imm(32, content.at(1));
      u_inst->R_IMM._imm = (INT32) content.at(1);
      break;
    case TY_AM64_R_OFS:
      Check_content_size(2, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_OFS._r1 = content.at(0);
      Check_imm(32, content.at(1));
      u_inst->R_OFS._ofs = content.at(1);
      break;
    case TY_AM64_R_R_IMM:
      Check_content_size(3, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_R_IMM._r1 = content.at(0);
      Check_reg(_10BIT_REG_MAX, content.at(1));
      u_inst->R_R_IMM._r2 = content.at(1);
      Check_imm(22, content.at(2));
      u_inst->R_R_IMM._imm = (INT32) content.at(2);
      break;
    case TY_AM64_R_R_OFS:
      Check_content_size(3, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_R_OFS._r1 = content.at(0);
      Check_reg(_10BIT_REG_MAX, content.at(1));
      u_inst->R_R_OFS._r2 = content.at(1);
      Check_imm(22, content.at(2));
      u_inst->R_R_OFS._ofs = content.at(2);
      break;
    case TY_AM64_R_R_R:
      Check_content_size(3, content_size);
      Check_reg(_10BIT_REG_MAX, content.at(0));
      u_inst->R_R_R._r1 = content.at(0);
      Check_reg(_10BIT_REG_MAX, content.at(1));
      u_inst->R_R_R._r2 = content.at(1);
      Check_reg(_10BIT_REG_MAX, content.at(2));
      u_inst->R_R_R._r3 = content.at(2);
      break;
    default:
      Is_True(false, ("Wrong ty am: %d.", ty_am));
      break;
  }
}

bool U_INST_UTIL::Is_32bit(TY_AM ty_am) {
  switch (ty_am) {
    case TY_AM32:
    case TY_AM32_OFS:
    case TY_AM32_R:
    case TY_AM32_R_IMM:
    case TY_AM32_R_OFS:
    case TY_AM32_R_R:
    case TY_AM32S_R_R_IMM:
    case TY_AM32S_R_R_OFS:
    case TY_AM32S_R_R_R:
      return true;
    case TY_AM64_R_IMM:
    case TY_AM64_R_OFS:
    case TY_AM64_R_R_IMM:
    case TY_AM64_R_R_OFS:
    case TY_AM64_R_R_R:
      return false;
    default:
      Is_True(false, ("Wrong ty am: %d.", ty_am));
  }
  return false;
}

void U_INST_UTIL::Print_reg_with_pt(PRINT_TYPE pt, UINT32 idx, FILE *file) {
  switch (pt) {
    case P_NONE:
      break;
    case P_I32:
    case P_U32:
      fprintf(file, "R%-9d", idx);
      break;
    case P_I64:
    case P_U64:
      fprintf(file, "RD%-8d", idx);
      break;
    case P_F32:
      fprintf(file, "F%-9d", idx);
      break;
    case P_F64:
      fprintf(file, "FD%-8d", idx);
      break;
    case P_OFS:
#ifdef __M64__
      fprintf(file, "RD%-8d", idx);
#else
      fprintf(file, "R%-9d", idx);
#endif
      break;
    default:
      Is_True(false, ("Unsupported print type: %d.", pt));
      break;
  }
}

void U_INST_UTIL::Print_reg_with_valty(VALTYPE valtype, UINT32 idx, FILE *file) {
  switch (valtype) {
    case I32:
      fprintf(file, "R%-9d", idx);
      break;
    case I64:
      fprintf(file, "RD%-8d", idx);
      break;
    case F32:
      fprintf(file, "F%-9d", idx);
      break;
    case F64:
      fprintf(file, "FD%-8d", idx);
      break;
    default:
      fprintf(file, "UNKNOWN_REGISTER(type:%d)%-8d", valtype, idx);
      break;
  }
}

void U_INST_UTIL::Print_reg(INT32 s_idx, INT32 t_idx, VALTYPE valtype, UINT32 idx, FILE *file) {
  if (s_idx != -1) {
    PRINT_TYPE pt = SPECIAL_FORMAT_ARR[s_idx].pt_arr[t_idx];
    Print_reg_with_pt(pt, idx, file);
  } else {
    Print_reg_with_valty(valtype, idx, file);
  }
}

INT32 U_INST_UTIL::Get_special_format(U_OPCODE_LONG opcode_l) {
  for (UINT32 i = 0; i < SPECIAL_FORMAT_ARR_LEN; i++) {
    if (SPECIAL_FORMAT_ARR[i].opcode_l == opcode_l) {
      return i;
    }
  }
  return -1;
}

