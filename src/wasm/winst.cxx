/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <cstdio>
#include "winst.h"
#include "reader.h"
#include "ir.h"
#include "type_enum.h"

#define ___ I32
#define WASM_OP(tr, st1, st2, st3, im1, im2, cls, numopr, ret_size, g, h, i, j) {i, g, im1, im2, cls, numopr},
WASM_INST WASM_META_INFO [] = {
#include "winst_opcode_list.h"
};
#undef ___
#undef WASM_OP

#include "op_enum.h"

/* This is quite slow, maybe we need to use a hashmap here. */
WASM_INST *OPCODE_inst(UINT8 opcode) {
  const int INST_TABLE_SIZE = sizeof(WASM_META_INFO) / sizeof(WASM_INST);
  for (UINT32 i = 0; i < INST_TABLE_SIZE; i++) {
    if (WASM_META_INFO[i].Encode() == opcode) {
      return &WASM_META_INFO[i];
    }
  }
  Is_True_Rel(false, ("Instruction not in the table : %d, 0x%02x", (INT32) opcode, (INT32) opcode));
  return nullptr;
}


UINT8 WASM_INST::Print_opr (FILE *file, VALTYPE opr_size, CONST_BUFFER pc) {
  switch(opr_size) {
    case VALTYPE::I32:{
      UINT32 opr1     = Read_leb128(pc);
      UINT8  leb_size = Read_opr_size_leb128(pc);
      fprintf(file, " 0x%08x", opr1);
      return leb_size;
    }
    case VALTYPE::I64: {
      UINT64 opr1     = Read_leb128(pc);
      UINT8  leb_size = Read_opr_size_leb128(pc);
      fprintf(file, " 0x%016llx", opr1);
      return leb_size;
    }
    case VALTYPE::F32: {
      UINT32 val = Read_const_value(pc, F32);
      fprintf(file, " %.16f (0x%08x)", reinterpret_cast<FLOAT32 &>(val), val);
      return 4;
    }
    case VALTYPE::F64: {
      UINT64 val = Read_const_value(pc, F64);
      fprintf(file, " %.16lf, (0x%016llx)", reinterpret_cast<FLOAT64 &>(val), val);
      return 8;
    }
    default:
      fprintf(file, " unknown type opr");
      return opr_size;
  }
}

UINT64 WASM_INST::Read_opr (UINT8 opr_size, CONST_BUFFER pc) {
  switch(opr_size) {
    case 4: {
      UINT32 opr1     = Read_leb128(pc);
      return opr1;
    }
    case 8: {
      UINT64 opr1     = Read_leb128(pc);
      return opr1;
    }
    case 0: return 0;
    default:
      return 0;
  }
}


INT32 WASM_INST::Print(CONST_BUFFER pc, FILE *file) {
  Is_True(Numopr() < 3, ("Numopr should be less than 3."));

  fprintf(file, "\t0x%018llx\t%02x", (UINT64) pc, Read_uint8(pc));
  UINT32 inst_size = Read_inst_size(pc);
  pc += 1;
  for (UINT32 i = 0 ; i < 9 || i < inst_size - 1; i++) {
    if (i < inst_size - 1) {
      fprintf(file, " %02x", pc[i]);
    } else {
      fprintf(file, "%-3s", "");
    }
  }
  int n = 0;
  fprintf(file, "\t// %s", _name);
  if (Numopr() > 0) {
    n += Print_opr(file, OpSz1(), pc);
    pc += n;
  }
  if (Numopr() > 1) {
    fprintf(file, ",");
    n += Print_opr(file, OpSz2(), pc);
  }
  fprintf(file, "\n");
  return n;
}


// emitting binary form of WASM to file.
//
INT32 WASM_INST::Emit_binary(CONST_BUFFER pc, FILE *file) {
  Is_True(Numopr() < 3, ("Numopr should be less than 3."));
  pc += 1;
  for (UINT32 i = 0 ; i < OpSz1() + OpSz2(); i++) {
    fprintf(file, "%c", pc[i]);
  }
  int n = 0;
  if (Numopr() > 0) {
    n += Emit_opr(file, OpSz1(), pc);
    pc += n;
  }
  if (Numopr() > 1) {
    n += Emit_opr(file, OpSz2(), pc);
  }
  return n;
}


// emitting operand
// 
UINT8 WASM_INST::Emit_opr (FILE *file, UINT8 opr_size, CONST_BUFFER pc) {
  UINT8 opr_real_bytes = 0;
  switch(opr_size) {
    case 4: {
      UINT32 opr1 = Read_leb128(pc);
      opr_real_bytes = Read_opr_size_leb128(pc);
      fwrite(&opr1, 1, opr_real_bytes, file);
      return 4;
    }
    case 8: {
      UINT64 opr1 = Read_leb128(pc);
      opr_real_bytes = Read_opr_size_leb128(pc);
      fwrite(&opr1, 1, opr_real_bytes, file);
      return 8;
    }
    case 0: return 0;
    default:
      return opr_size;
  }
}

UINT32 WASM_INST::Read_inst_size(CONST_BUFFER pc) {
  pc += 1;
  UINT32 n = 0;
  if (Numopr() > 0) {
    n = Read_opr_size_valty(pc, OpSz1());
    pc += n;
  }
  if (Numopr() > 1) {
    n += Read_opr_size_valty(pc, OpSz2());
  }
  // Please reference the official document for details on this process.
  // The encoding is
  // br_table vec(l* :labelidx) Ln: labelidx,
  // where vec(B) := n:u32, x:(B)^n.....
  if (Encode() == BrTable) {
    Is_True(Numopr() == 0, ("Brtable should have numopr = 0"));
    // Read the vec(l*: labelidx)
    UINT32 vec_size = Read_leb128(pc); // read the vector's size
    UINT32 vec_size_field_len = Read_opr_size_leb128(pc);
    pc += vec_size_field_len; // move after the size field.
    n += vec_size_field_len;
    for(int i = 0; i < vec_size; i++) {
      // Read each labelidx
      UINT32 next_opr_size = Read_opr_size_leb128(pc);
      n += next_opr_size;
      pc += next_opr_size;
    }
    // Read the final Ln:labelidx
    UINT32 next_opr_size = Read_opr_size_leb128(pc);
    n += next_opr_size;
  }
  return n + 1;
}

UINT32 WASM_INST::Read_opr_size_valty(const UINT8 *pc, VALTYPE valty) const {
  UINT32 size = 0;
  switch(valty) {
    case I32:
    case I64:
      size = Read_opr_size_leb128(pc);
      break;
    case F32:
      size = 4;
      break;
    case F64:
      size = 8;
      break;
    default:
      Is_True(false, ("Not recognized opsz : %d", OpSz2()));
  }
  return size;
}

INT32 WASM_INST::Print(CONST_BUFFER pc, UINT32 ins_cnt, FILE *file) {
  fprintf(file, "\t0x%06x\t", ins_cnt);
  return Print(pc, file);
}


INT32 WASM_INST::Print(FILE *file) {
  return Print((CONST_BUFFER) this->Pc(), file);
}

INT32 WASM_INST::Print(UINT32 ins_cnt, FILE *file) {
  fprintf(file, "\t0x%06x\t", ins_cnt);
  return Print(file);
}
