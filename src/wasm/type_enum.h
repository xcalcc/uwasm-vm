/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/1.
//

#ifndef WASM2UWASM_TYPE_ENUM_H
#define WASM2UWASM_TYPE_ENUM_H

#include "u_am.h"
#include "../utilities/u_utils.h"

enum WABT_ENUM {
  WABT_I32     = -0x01,      // 0x7f
  WABT_I64     = -0x02,      // 0x7e
  WABT_F32     = -0x03,      // 0x7d
  WABT_F64     = -0x04,      // 0x7c
  WABT_V128    = -0x05,     // 0x7b
  WABT_I8      = -0x06,       // 0x7a  : packed-type only, used in gc and as v128 lane
  WABT_I16     = -0x07,      // 0x79  : packed-type only, used in gc and as v128 lane
  WABT_Funcref = -0x10,  // 0x70
  WABT_Anyref  = -0x11,   // 0x6f
  WABT_Nullref = -0x12,  // 0x6e
  WABT_Exnref  = -0x18,   // 0x68
  WABT_Func    = -0x20,     // 0x60
  WABT_Struct  = -0x21,   // 0x5f
  WABT_Array   = -0x22,    // 0x5e
  WABT_Void    = -0x40,     // 0x40
  WABT_VOID    = -0x40,     // 0x40
  WABT____     = WABT_Void,       // Convenient for the opcode table in opcode.h
  WABT_Any     = 0,          // Not actually specified, but useful for type-checking
  WABT_Hostref = 2,      // Not actually specified, but used in testing and type-checking
  WABT_I8U     = 4,   // Not actually specified, but used internally with load/store
  WABT_I16U    = 6,  // Not actually specified, but used internally with load/store
  WABT_I32U    = 7,   // Not actually specified, but used internally with load/store
};

struct WABT_to_OPTYPE_info {
  WABT_ENUM wabttype;
  VALTYPE optype;
};


static WABT_to_OPTYPE_info wabt_op_tab [] = {
#define UWASM_TY_DEF(e_name, e_val, wabt_val) {wabt_val, e_name},
#include "type_enum_base.h"
#undef UWASM_TY_DEF
};


// this is defined in basics.cxx
VALTYPE Conv_wabt_ty(UINT32 wabt_enum);

WABT_ENUM Conv_ty_wabt(UINT32 valty);

//VALTYPE implement_type[IMPLEMENT_TYPE_NUM] = { I32, I64, F32, F64 }; // for future use

#endif //WASM2UWASM_TYPE_ENUM_H
