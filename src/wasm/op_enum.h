/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/7/22.
//
#include "type_enum.h"

#ifndef WASM2UWASM_OP_ENUM_H
#define WASM2UWASM_OP_ENUM_H

#define WASM_OP(a,b,c,d,r1,r2,e,numopr,ret_size, encode_8bit, enum_name,i,j) enum_name = encode_8bit,

enum WINST_OPCODE {
#include "winst_opcode_list.h"
};

#undef WASM_OP
#undef UWASM_OP
#undef UWASM_TAB

#endif //WASM2UWASM_OP_ENUM_H
