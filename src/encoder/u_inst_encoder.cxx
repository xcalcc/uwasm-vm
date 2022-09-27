/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "u_inst_encoder.h"

void U_CODE_BODY::Print(FILE *file) {
  _ubh_code.Set_inst_tab(_inst_tab.empty() ? NULL : &_inst_tab[0]);
  _ubh_code.Set_label_tab(_lab_tab.empty() ? NULL : (UINT32 *) &_lab_tab[0]);
  _ubh_code.Set_code_size(_inst_tab.size());
  _ubh_code.Set_label_num(_lab_tab.size());
  _ubh_code.Print(file);
}

void U_CODE_BODY::Print_header(FILE *file) {
  _ubh_code.Set_inst_tab(_inst_tab.empty() ? NULL : &_inst_tab[0]);
  _ubh_code.Set_label_tab(_lab_tab.empty() ? NULL : (UINT32 *) &_lab_tab[0]);
  _ubh_code.Set_code_size(_inst_tab.size());
  _ubh_code.Set_label_num(_lab_tab.size());
}

UINT32 U_CODE_BODY::Request_label(const char *label_name) {
  if (_lab_name_tab.Contains(label_name)) {
    return _lab_name_tab.Get_idx(label_name);
  }
  UINT32 label_name_idx = _lab_name_tab.Add(label_name);
  _lab_tab.push_back(UNINIT_LABEL_IDX);
  // label name table size is equal to label table size all the time
  Is_True(_lab_name_tab.Get_entry_vec().size() == _lab_tab.size(),
          ("Label name table size is not equal to label tab size, label name table size: %d, label table size: %d.",
            _lab_name_tab.Get_entry_vec().size(), _lab_tab.size()));
  return label_name_idx;
}

void U_CODE_BODY::Finish(UINT32 entry_off) {
  // Check if label table contains incomplete entries.
  for (UINT32 i = 0; i < _lab_tab.size(); i++) {
    Is_True(_lab_tab.at(i) != UNINIT_LABEL_IDX, ("Found an incomplete label entry, label_id = %d, name = %s", i, _lab_name_tab.Get_entry_str(i)));
  }
  UINT64 off = entry_off;
  _ubh_code.Set_label_num(_lab_tab.size());
  _ubh_code.Set_code_size(_inst_tab.size());
  off += sizeof(UBH_CODE);
  _ubh_code.Set_inst_tab_off(off);
  off += sizeof(UINT32) * _inst_tab.size();
  _ubh_code.Set_label_tab_off(off);
}

UINT32 U_INST_ENCODER::Allocate_code_body(UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size) {
  _u_code_vec.emplace_back(local_regs, io_regs, stack_size);
  return _u_code_vec.size() - 1;
}

void U_INST_ENCODER::Finish() {
  UINT32 entry_off = 0;
  for (UINT32 i = 0; i < _u_code_vec.size(); i++) {
    U_CODE_BODY &u_code_body = _u_code_vec[i];
    u_code_body.Finish(entry_off);
    entry_off += u_code_body.Entry_size();
  }
}

void U_INST_ENCODER::Write_to_buf(char *buf) {
  char *buf_p = buf;
  for (UINT32 i = 0; i < U_code_vec().size(); i++) {
    U_CODE_BODY &u_code_body = U_code_vec()[i];
    UINT32 size = sizeof(UBH_CODE);
    memcpy(buf_p, &u_code_body.Ubh_code(), size);
    buf_p += size;
    size = u_code_body.Inst_tab().size() * sizeof(UINT32);
    // copy inst
    if (size != 0) {
      memcpy(buf_p, &(u_code_body.Inst_tab()[0]), size);
    }
    buf_p += size;
    size = u_code_body.Lab_tab().size() * sizeof(UINT32);
    // copy label
    if (size != 0) {
      memcpy(buf_p, &(u_code_body.Lab_tab()[0]), size);
    }
    buf_p += size;
  }
}

UINT32 U_INST_ENCODER::Code_sec_size() {
  UINT32 size = 0;
  for (UINT32 i = 0; i < U_code_vec().size(); i++) {
    size += U_code_vec()[i].Entry_size();
  }
  return size;
}
