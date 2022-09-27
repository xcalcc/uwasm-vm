/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_EXEC_PERF_H
#define UWASM_VM_EXEC_PERF_H

#include <chrono>
#include <cmath>

#include "u_types.h"
#include "ubh/ubh.h"
#include "interpreter/exec_unit.h"
#include "runtime/exec_system.h"
#include "ubh/u_opcode.h"

extern const char *inst_impl_func_name[];
class UWASM_PERF {
  friend class UWASM_MACHINE;

private:
  // Performance utility
  UINT64 _opcode_usage_info[MAXOPCODE_L + 1];

  UINT64 _total_instrs = 0;

  UINT32 _ret_val;

  std::chrono::steady_clock::time_point _begin_time;

public:
  UINT64 Total_instrs() { return _total_instrs; }

  // Record the statistics on opcode_l.
  //
  void Record_op(UINT32 opcode) {
    _opcode_usage_info[opcode] ++;
    _total_instrs ++;
    Is_True(_opcode_usage_info != NULL && opcode < MAXOPCODE_L + 1, ("Incorrect state for recording perf data."));
  }

  void Init() {
    for (UINT32 i = 0; i < MAXOPCODE_L; i++) {
      _opcode_usage_info[i] = 0;
    }
    _total_instrs = 0;
    _begin_time = std::chrono::steady_clock::now();
  }

  void Set_ret_val(UINT32 ret_val) { _ret_val = ret_val; }

  void Print(FILE *file = TFile) {
    Is_Trace_cmd_rel(Tracing(TP_EXEC), {
      fprintf(file, "Instr executed : %llu, final ret = %d, \n", Total_instrs(), _ret_val);
    });
    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    double time_spent = std::chrono::duration_cast<std::chrono::microseconds>(end_time - _begin_time).count();
    double ips = ((double) Total_instrs() * 1000.0 * 1000.0) / time_spent;
    Is_Trace_cmd_rel(Tracing(TP_EPERF), {
      for (UINT32 i = 0; i < MAXOPCODE_L; i++) {
        if (_opcode_usage_info[i] == 0)
          continue;

        // Print the info for this opcode_l.
        fprintf(file, "%s, %llu\n", inst_impl_func_name[i], _opcode_usage_info[i]);
      }
      fprintf(file, "Spent time = %.5lfms, IPS = %.2lf, Final ret = %d\n", (time_spent / 1000.0), ips, _ret_val);
    });
  }
};

static UWASM_PERF *uvm_perf_tracker = NULL;

UWASM_PERF & Uvm_perf_tracker() {
  if (!uvm_perf_tracker) {
    uvm_perf_tracker = new UWASM_PERF();
  }
  return *uvm_perf_tracker;
}

#endif //UWASM_VM_EXEC_PERF_H
