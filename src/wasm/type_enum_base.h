/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/2.
//


UWASM_TY_DEF(I32,      0x00,   WABT_I32)   // 0x7f
UWASM_TY_DEF(I64,      0x01,   WABT_I64)   // 0x7e
UWASM_TY_DEF(F32,      0x02,   WABT_F32)   // 0x7d
UWASM_TY_DEF(F64,      0x03,   WABT_F64)   // 0x7c
//UWASM_TY_DEF(V128,     0x10,   WABT_V128)   // 0x7b
//UWASM_TY_DEF(I8,       0x11,   WABT_I8)   // 0x7a  : packed-type only, used in gc and as v128 lane
//UWASM_TY_DEF(I16,      0x07,   WABT_I16)  // 0x79  : packed-type only, used in gc and as v128 lane
//UWASM_TY_DEF(Funcref,  0x10,   WABT_Funcref)  // 0x70
//UWASM_TY_DEF(Anyref,   0x11,   WABT_Anyref)  // 0x6f
//UWASM_TY_DEF(Nullref,  0x12,   WABT_Nullref)  // 0x6e
//UWASM_TY_DEF(Exnref,   0x18,   WABT_Exnref)  // 0x68
//UWASM_TY_DEF(Func,     0x20,   WABT_Func)  // 0x60
//UWASM_TY_DEF(Struct,   0x21,   WABT_Struct)  // 0x5f
//UWASM_TY_DEF(Array,    0x22,   WABT_Array)  // 0x5e
//UWASM_TY_DEF(Void,     0x40,   WABT_Void)  // 0x40
//UWASM_TY_DEF(VOID,     Void,   WABT_VOID)   // 0x40
//UWASM_TY_DEF(___,      Void,   WABT____)   // Convenient for the opcode table in opcode.h
//UWASM_TY_DEF(Any,      0x31,   WABT_Any)  // Not actually specified, but useful for type-checking
//UWASM_TY_DEF(Hostref,  0x32,   WABT_Hostref)  // Not actually specified, but used in testing and type-checking
//UWASM_TY_DEF(I8U,      0x34,   WABT_I8U)  // Not actually specified, but used internally with load/store
//UWASM_TY_DEF(I16U,     0x36,   WABT_I16U)  // Not actually specified, but used internally with load/store
//UWASM_TY_DEF(I32U,     0x37,   WABT_I32U)  // Not actually specified, but used internally with load/store