/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/13.
//

#ifndef UWASM_VM_CONV_MAP_H
#define UWASM_VM_CONV_MAP_H

#include "ubh/u_inst.h"
#include "../utilities/u_utils.h"
#include "u_am.h"
#include "op_enum.h"

typedef UINT32 REG_IDX;

// UWASM instruction info
//
U_OPC_TY_PAIR UOPCODE_inst(UINT16 opcode, VALTYPE type);
U_OPC_TY_PAIR UOPCODE_from_wasm(UINT16 opcode);
extern U_BUILTIN_CODE Conv_wasm_opc_to_builtin(UINT16 opc);

class OPCODE_MAP_ELEM {
private:
  WINST_OPCODE wasm_op;
  U_OPCODE uwasm_op;
  VALTYPE type;
public:
  OPCODE_MAP_ELEM(WINST_OPCODE wasm_op, U_OPCODE uwasm_op, VALTYPE type) :
    wasm_op(wasm_op),     uwasm_op(uwasm_op),     type(type) {}

  WINST_OPCODE  Get_wasm_op()  const { return wasm_op;  }
  U_OPCODE Get_uwasm_op() const { return uwasm_op; }
  VALTYPE     Get_type()     const { return type;     }
};


// Auxillary structure for storing the mapping info
// between short and long version of the same instruction
// normally used for LOAD/STOREs with offset to be variable
class UWASM_OP_SL_CONV {
private:
    UINT32  _short_enum = 0; // short version
    UINT32  _long_enum  = 0; // long version
public:
    UINT32 Get_short() const { return _short_enum; }
    UINT32 Get_long()  const { return _long_enum;  }
    UWASM_OP_SL_CONV(UINT32 s, UINT32 l) {
      _short_enum = s;
      _long_enum = l;
    }
};

UINT32 Conv_to_long_opcode(UINT32 short_opcode);

#define OFFSET_S_MAX ((1ull << 8) - 1)
#define OFFSET_M_MAX ((1ull << 22) - 1)
#define OFFSET_L_MAX ((1ull << 32) - 1)
#define REG_S_MAX    ((1ull << 8) - 1)
#define REG_L_MAX    ((1ull << 10) - 1)

#endif //UWASM_VM_CONV_MAP_H
