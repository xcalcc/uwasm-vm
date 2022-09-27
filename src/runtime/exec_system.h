/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/9/12.
//
#include <string.h>
#include "interpreter/exec_unit.h"
#include "interpreter/uwasm_malloc.h"


#ifndef UWASM_VM_EXEC_SYSTEM_H
#define UWASM_VM_EXEC_SYSTEM_H

class UWASM_SYSTEM;

class UWASM_SYSTEM {
private:
  UWASM_MACHINE                       _machine;

public:
  UWASM_MACHINE  &Machine()                                           { return _machine; }
  UINT32          Init_memory(char *mem_p, UINT32 mem_size)           { return _machine.Init_memory(mem_p, mem_size); }
  U_MODULE       *Load_from_buffer(char *buffer, UINT32 buffer_len);
  U_MODULE       *Load_from_file(const char *filename);
  void            Set_cur_func(UINT32 func_idx)                       { _machine.Init_env(func_idx); }
  INT32           Search_start();
  void            Exec_continue();
  void            Print(FILE *file);
  void            Debug_next_instr(INT32 i);
  void            Debug_set_reg_i32(UINT32 index, INT32 value)        { _machine.Reg_i32().Set(index, value); }
  void            Debug_set_reg_i64(UINT32 index, INT64 value)        { _machine.Reg_i64().Set(index, value); }
  void            Debug_set_reg_f32(UINT32 index, FLOAT32 value)      { _machine.Reg_f32().Set(index, value); }
  void            Debug_set_reg_f64(UINT32 index, FLOAT64 value)      { _machine.Reg_f64().Set(index, value); }
  INT32           Search_start_by_name(const char *func_name);
  void            Set_string_args(vector<string> &args)               { _machine.Get_string_args().assign(args.begin(), args.end()); }
};

UWASM_SYSTEM *Uvm_system();

#endif //UWASM_VM_EXEC_SYSTEM_H
