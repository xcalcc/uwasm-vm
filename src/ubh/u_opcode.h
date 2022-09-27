/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/8.
//

#ifndef UWASM_VM_UWINST_OPCODE_H
#define UWASM_VM_UWINST_OPCODE_H

#include "u_am.h"

enum U_OPCODE {
#define UOPCODE(opcode, op_enum, name, ty_am) op_enum = opcode,
#include "u_opcode_list.def"
#undef UOPCODE
};

enum U_OPCODE_LONG {
#define U_OPCODE(opcode) opcode,
#include "ubh/u_opcode_list_l.def"
#undef U_OPCODE
  MAXOPCODE_L,
};

// Builtin's special opcode
enum U_BUILTIN_CODE {
#define U_BUILTIN(opcode, value, wasmcode, wasmvalue, name) opcode = value,
#include "ubh/u_builtin_list.def"
#undef U_BUILTIN
  MAXBUILTIN_CODE,
};

extern const char *UWINST_NAME[];
extern TY_AM UWINST_TY_AM[];
extern const char *U_BUILTIN_CODE_NAME[];

#endif //UWASM_VM_UWINST_OPCODE_H
