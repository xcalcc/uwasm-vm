/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by ark.fang on 2020/8/13.
//
#include "u_opcode.h"

const char *UWINST_NAME[] = {
#define UOPCODE(opcode, op_enum, name, ty_am) name,
#include "u_opcode_list.def"
#undef UOPCODE
};


// UWINST_TY_AM[opcode_l] is the am type of an opcode_l
//
TY_AM UWINST_TY_AM[] = {
#define UOPCODE(opcode, op_enum, name, ty_am) ty_am,
#include "u_opcode_list.def"
#undef UOPCODE
};

// Builtin code's name.
//
const char *U_BUILTIN_CODE_NAME[] = {
#define U_BUILTIN(opcode, value, wasmcode, wasmvalue, name) name,
#include "u_builtin_list.def"
#undef U_BUILTIN
};