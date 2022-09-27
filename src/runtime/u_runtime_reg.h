//
// Created by jason on 3/27/21.
//

#ifndef UWASM_VM_U_RUNTIME_REG_H
#define UWASM_VM_U_RUNTIME_REG_H

// These are conventions shared by runtime & code generators / wasm transcoder.

#ifdef UWASM_VM_RUNTIME_WASM
#define CONST_REG_NUM     0
#define RET_REG_NUM       2 // reg use for return, r0/rd0/rf0/rfd0 => return-reg
#else
#define CONST_REG_NUM     0
#define RET_REG_NUM       1 // reg use for return, r0/rd0/rf0/rfd0 => return-reg
#endif

#endif //UWASM_VM_U_RUNTIME_REG_H
