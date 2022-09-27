/*
 * Copyright 2018 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
//
// Edited by @Jason on 2020/11/6.
// Moved the following lines from opcode-code-table.cc to
// opcode-code-table-extra.c for allowing GCC to compile this source file.
// Because the initialization syntax in WabtOpcodeCodeTable is not supported
// in GCC's C++ lingual.
//
#include <stdlib.h>
#include <stdint.h>

#define WABT_OPCODE_CODE_TABLE_SIZE 65536

#include "config.h"

typedef enum WabtOpcodeEnum_C {
#define WABT_OPCODE(rtype, type1, type2, type3, mem_size, prefix, code, Name, \
                    text, decomp)                                             \
  Name,
#include "opcode.def"
#undef WABT_OPCODE
  Invalid,
} WabtOpcodeEnum_C_ONLY;

/* The array index calculated below must match the one in Opcode::FromCode. */
uint32_t WabtOpcodeCodeTable[WABT_OPCODE_CODE_TABLE_SIZE] = {

// this feature was not supported by c++ standard, need to initialize all members
// or change to c file, and linked them together
#define WABT_OPCODE(rtype, type1, type2, type3, mem_size, prefix, code, Name, \
                    text, decomp)                                             \
  [(prefix << 8) + code] = Name,
#include "opcode.def"
#undef WABT_OPCODE
};
