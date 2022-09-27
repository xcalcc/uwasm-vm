/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <cstdio>
#include "conv_map.h"
#include "conv_trace.h"
#include "u_trace.h"

#define UWASM_TAB(name, uwasm_op, ty_am, ty) {name, uwasm_op, ty_am, VALTYPE::ty},
#define GET_MEM_VAL(val, n) (*(UINT8 *)(((char *) val) + n))
#define OP_MAP(wasm_op, uwasm_op, type) {wasm_op, uwasm_op, type},
#define UWASM_OP(opcode, op_enum, name, ty_am) {name, opcode, ty_am},

OPCODE_MAP_ELEM op_map[] = {
#include "op_map.h"
};

U_OPC_TY_PAIR UOPCODE_inst(UINT16 opcode, VALTYPE type) {
  U_OPC_TY_PAIR instr = U_OPC_TY_PAIR(opcode, (VALTYPE) type);
  return instr;
}

U_OPC_TY_PAIR UOPCODE_from_wasm(UINT16 opcode) {
  for (auto i : op_map) {
    if (i.Get_wasm_op() == opcode) {
      UINT16 uop = i.Get_uwasm_op();
      Is_Trace(Convtrace(TCK_INSTR_CONV),
               (TFile, "Conv: wasmop = 0x%02x to uwasmop = %d(0x%02x) %s\n",
                 opcode, (UINT32)uop, (UINT32)uop, UWINST_NAME[uop]));
      return U_OPC_TY_PAIR(uop, i.Get_type());
    }
  }
  Is_True(false,
          ("Wasm-opcode(0x%02x) not convertible to uwasm opcode", opcode));
  return U_OPC_TY_PAIR(0, VALTY_MIN);
}


#define OP_SLMAP(codea, enuma, namea, tyama, codeb, enumb, nameb, tyamb) { enuma, enumb },
UWASM_OP_SL_CONV UWASM_OP_SL_MAP [] = {
#include "op_short_long_map.h"
};
#undef OP_SLMAP


// Convert uwasm short opcode to long opcode using table.
//
UINT32 Conv_to_long_opcode(UINT32 short_opcode) {
  for (auto op_sl_entry : UWASM_OP_SL_MAP) {
    if (op_sl_entry.Get_short() == short_opcode) {
      return op_sl_entry.Get_long();
    }
  }
  Is_True_Rel(false, ("Failed to find the long version of short_opcode = %d", short_opcode));
  return 0;
}


// Wasm instruction opcode to builtin code mapping.
//
class WINST_OPCODE_BUILTIN_PAIR {
  WINST_OPCODE   _wopc;
  U_BUILTIN_CODE _u_builtin;
public:
  WINST_OPCODE_BUILTIN_PAIR(WINST_OPCODE opc, U_BUILTIN_CODE ub):
    _wopc(opc), _u_builtin(ub) {}
  WINST_OPCODE   Get_winst_opc()    { return _wopc;        }
  U_BUILTIN_CODE Get_builtin_code() { return _u_builtin;   }
};


WINST_OPCODE_BUILTIN_PAIR WINST_OPCODE_BUILTIN[] = {
#define U_BUILTIN(code_name, value, wasmcode, wasmvalue, name) {wasmcode, code_name},
#include "ubh/u_builtin_list.def"
#undef U_BUILTIN
};


// Convert wasm opcode to builtin code using the mapping table.
//
U_BUILTIN_CODE Conv_wasm_opc_to_builtin(UINT16 opc) {
  for (auto op_sl_entry : WINST_OPCODE_BUILTIN) {
    if (op_sl_entry.Get_winst_opc() == opc) {
      return op_sl_entry.Get_builtin_code();
    }
  }
  Is_True(false, ("Failed to find the builtin code for wasm opcode = %d or 0x%08x, use UNKNOWN for now.", opc, opc));
  return U_BUILTIN_CODE::UBT_UNKNOWN;
}