/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "reader.h"
#include "op_enum.h"
#include <sstream>
#include "converter.h"
#include "u_leb128.h"

INT64 Read_signed_leb128(CONST_BUFFER pc) {
  INT64 res = decodeSLEB128(pc);
  return res;
}

UINT64 Read_leb128(CONST_BUFFER pc) {
  UINT64 result = 0;
  UINT32 offset = 0; // 7, 15, 23, 31, 39, 47, 55, 63,
  for (UINT32 current = *(UINT8 *)pc; (current >> 7) != 0; pc++, current = *(UINT8 *)pc) {
    result += (current & 0x7f) << offset;
    offset += 7;
  }
  result += ((UINT32) *(UINT8 *)pc & 0x7f) << offset;
  return result;
}


UINT8 Read_opr_size_leb128(CONST_BUFFER pc) {
  UINT8 result = 1;
  for (UINT32 current = *(UINT8 *)pc; ((current >> 7) & 0x01) != 0; pc++, current = *(UINT8 *)pc) {
    result ++;
  }
  return result;
}

UINT8  Read_uint8(CONST_BUFFER pc) {
  UINT8 res = *pc;
  return res;
}

// Read a constant with specified valtype, I32/I64/F32/F64.
// return value is represented in a uint64 format. (f32/f64 values will be reinterpreted as UINT format to preseve its value)
//
UINT64 Read_const_value(CONST_BUFFER pc, VALTYPE valty) {
  switch(valty) {
    case I32:
    case I64: {
      return (UINT64) Read_signed_leb128(pc);
    }
    case F32: {
      return (UINT64) *(UINT32 *) pc;
    }
    case F64: {
      return (UINT64) *(UINT64 *) pc;
    }
    default:{
      Is_True_Rel(false, ("Incorrect valty used : %d", valty));
    }
  }
}


using std::vector;


INT32 Read_magic_number(CONST_BUFFER pc) {
  Is_True(memcmp(pc, "\0asm\1\0\0\0\0", 8) == 0,
          ("Incorrect magic number"));
  return 0;
}


INT32 Read_all_sections(UINT8 *buffer, UINT32 length, FILE_CONTEXT& ctx) {
  CONST_BUFFER pc  = buffer;
  CONST_BUFFER end = pc + length;
  if (ctx.Is_full_file()) {
    Read_magic_number(buffer);
    for(;pc < end;) {
      UINT8 section_id = Read_uint8(buffer);
      switch (section_id) {
        case 0x01: {
          pc++;
        }
      }
    }
  }
  // Treat the whole buffer as instruction buffer
  Read_insts(pc, length, ctx);
  return 0;
}


void Save_wasm_pc(CONST_BUFFER pc, WASM_INST &inst) {
  Is_True(inst.Numopr() < 3, ("Numopr should be less than 3."));
  inst.Set_pc((UINT64) pc);
}


WINSVEC Read_insts(CONST_BUFFER inst_buf, UINT32 buf_len, FILE_CONTEXT &ctx) {
  vector<WASM_INST> wasm_insts;
  CONST_BUFFER pc      = inst_buf;
  CONST_BUFFER end_buf = inst_buf + buf_len;
  Is_Trace(Convtrace(TCK_WASM_READER), (TFile, "Reading the wasm code section.\n"));
  for (; pc < end_buf;) {
    UINT8 opcode = Read_uint8(pc);
    WASM_INST wasm_inst = *(OPCODE_inst(opcode));
    Save_wasm_pc(pc, wasm_inst);
    if (Convtrace(TCK_WASM_READER)) wasm_inst.Print(pc, TFile);
    CONST_BUFFER orig_pc = pc;
    pc += wasm_inst.Read_inst_size(pc);
    // Variable length instruction here.
    wasm_insts.push_back(wasm_inst);
  }
  return wasm_insts;
}

std::string FILE_CONTEXT::Get_anon_func_name() {
  std::stringstream ss;
  ss << "f" << Get_last_func_idx();
  std::string cur_func_name = ss.str();
  Get_last_func_idx() ++;
  return cur_func_name;
}

std::string FILE_CONTEXT::Get_anon_global_name() {
  std::stringstream ss;
  ss << "g" << Get_last_func_idx();
  std::string cur_func_name = ss.str();
  Get_last_func_idx() ++;
  return cur_func_name;
}
