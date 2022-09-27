/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_INST_ENCODER_H
#define UWASM_VM_U_INST_ENCODER_H

#include <vector>
#include "u_types.h"
#include "ubh/ubh.h"
#include "utilities/u_str_tab.h"
#include "ubh/u_inst.h"

using namespace std;

class U_CODE_BODY {
private:
  UBH_CODE          _ubh_code;
  vector<UINT32>    _inst_tab;
  vector<INT32>     _lab_tab;
  U_STR_TAB         _lab_name_tab;

public:
  U_CODE_BODY(UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size):
    _ubh_code(local_regs, io_regs, stack_size) {}
  void Add_inst32(UINT32 inst)                          { _inst_tab.push_back(inst); }
  void Add_inst64(UINT64 inst) {
    _inst_tab.push_back((UINT32) inst);
    _inst_tab.push_back((UINT32) (inst >> 32u));
  }
  UINT32            Request_label(const char *label_name);
  void Set_label(UINT32 label_idx) {
    Is_True(label_idx < _lab_tab.size(),
            ("Label index out of bound, label table size: %d, label index: %d.", _lab_tab.size(), label_idx));
    _lab_tab.at(label_idx) = _inst_tab.size();
  }
  // must call finish before write to ubh file
  void              Finish(UINT32 entry_off);
  const UBH_CODE   &Ubh_code()                    const { return _ubh_code; }
  vector<UINT32>   &Inst_tab()                          { return _inst_tab; }
  vector<INT32>    &Lab_tab()                           { return _lab_tab; }
  UINT32            Entry_size()                        { return sizeof(UBH_CODE) + sizeof(UINT32) * (_inst_tab.size() + _lab_tab.size()); }
  UINT32            Get_inst_size()                     { return _inst_tab.size(); }
  U_INST_ITER       begin()                             { return _inst_tab.empty() ? U_INST_ITER(NULL) : U_INST_ITER(&_inst_tab[0]); }
  U_INST_ITER       end()                               { return _inst_tab.empty() ? U_INST_ITER(NULL) : U_INST_ITER(&(*_inst_tab.end())); }
  void              Print(FILE *file) ;
  void              Update_ubh_code(UINT32 *local_regs,
                                    UINT32 *io_regs,
                                    UINT32 stack_size) {
    _ubh_code.Update_ubh_code(local_regs, io_regs, stack_size);
  }

  void Print_header(FILE *file);
};

class U_INST_ENCODER {
private:
  vector<U_CODE_BODY>      _u_code_vec;

public:
  UINT32                   Allocate_code_body(UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size);
  U_CODE_BODY             &Cur_code_body()              { Is_True(_u_code_vec.size() > 0, ("No code body here.")); return _u_code_vec.back(); }
  vector<U_CODE_BODY>     &U_code_vec()                 { return _u_code_vec; }
  void                     Finish();
  void                     Write_to_buf(char *buf);
  UINT32                   Code_sec_size();
};

#endif //UWASM_VM_U_INST_ENCODER_H
