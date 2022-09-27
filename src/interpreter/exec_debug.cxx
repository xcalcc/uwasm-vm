//
// Created by xc5 on 4/2/21.
//

#include "exec_debug.h"

EXEC_CONTEXT * UWASM_DEBUG::Get_current_frame() {
  return _current_dbg_frame;
}
EXEC_CONTEXT * UWASM_DEBUG::Change_current_frame(UINT32 id) {
  Is_True(_machine != nullptr, ("Debugger not initialized."));
  // 0 ==> cur_env();
  if (id == 0) {
    _current_dbg_frame = &_machine->Cur_env();
    _current_dbg_id = 0;
    Print_one_frame(0, _current_dbg_frame);
  } else if (id > _machine->Get_stacktrace().TopIdx()) {
    // Error, probably not viable
    // There maybe errors ahead;
    Is_True(id < _machine->Get_stacktrace().Capacity(),
            ("frame index out of bound. = %d", id));
  } else {
    UINT32 top = _machine->Get_stacktrace().TopIdx();
    INT32 target = top - id;
    Is_True(target >= 0 && target <= top, ("Frame id out of bound. id = %d, top = %d", id, top));
    _current_dbg_frame = &_machine->Get_stacktrace().Vector().at(target);
    _current_dbg_id = id;
    Print_one_frame(id, _current_dbg_frame);
  }
}

void UWASM_DEBUG::Print_one_frame(UINT32 id, EXEC_CONTEXT *ctx) {
  UINT32 fid, ubhpc, func_id;
  func_id = _current_dbg_frame->Get_func_idx();
  // FID: funcname (args)  at <filename>:<linenum>
  // #0  main (argc=1, argv=0x7fffffffd738) at /home/xc5/xc5/javascript/uwasm-vm/src/main.cxx:393
  // #ID  funcname
  AM_EXPAND *exp = _current_dbg_frame->Get_pc();
  _machine->Get_u_module().Map_to_ubh_pc(exp, fid, ubhpc);
  UINT32 func_name_idx = _machine->Get_u_module().Get_func(func_id).Get_name_idx();
  const char *func_name = _machine->Get_u_module().Get_module_name(func_name_idx);
  // func_name = ;
  fprintf(_output_file,
          "#%d %s(funcid = %u) uwasm:%u, expand:%p\n",
          id, func_name, func_id, ubhpc, exp);
}

void UWASM_DEBUG::Print_current_frame() {
  Print_one_frame(_current_dbg_id, _current_dbg_frame);
  // Print arguments
  // Print registers
}

void UWASM_DEBUG::Print_current_func() {
  // Print function.
  Print_func(_current_dbg_frame->Get_func_idx());
}

void UWASM_DEBUG::Print_func(UINT32 func_idx) {
  // Print function.
  _machine->Get_u_module().Print_func_header(func_idx, _output_file);
}

void UWASM_DEBUG::Print_callstack() {
  UINT32 top = _machine->Get_stacktrace().TopIdx();
  Print_one_frame(0, &_machine->Cur_env());
  for (UINT32 i = 1; i <= top; i++) {
    INT32 target = (INT32) top - i;
    Is_True(target >= 0 && target <= top, ("bt: Frame id out of bound. id = %d, top = %d", i, top));
    Print_one_frame(i, &_machine->Get_stacktrace().Vector().at(target));
  }
}

void UWASM_DEBUG::On_breakpoint(AM_EXPAND *exp) {
  if (!Is_inst_breakable(exp)) {
    return;
  }
  const vector<UWASM_BP> &bps = Get_breakpoints();
  UINT32 cur_bps = 0;
  for (UINT32 i = 0; i < bps.size(); i++) {
    if (bps.at(i).Get_inst_pc() == exp) {
      cur_bps = i;
      break;
    }
  }
  // set up break point situations.
  Change_current_frame(0);
  _cur_pc = exp;
  // Find which bp triggered.
  fprintf(_output_file, "Breakpoint #%d: at func(%d), ubh_pc = %d, exp = %p\n",
          bps.at(cur_bps).Get_bp_id(),
          bps.at(cur_bps).Get_func_id(),
          bps.at(cur_bps).Get_ubh_pc(),
          bps.at(cur_bps).Get_inst_pc());
}

FILE *UWASM_DEBUG::Get_stderr() {
  return stderr;
};

void UWASM_DEBUG::Load_file_and_module(U_MODULE &module) {
  _current_dbg_frame = nullptr;
  _current_dbg_id = 0;
  _current_dbg_pc._hashval = 0;
  _cur_pc = nullptr;
}

void UWASM_DEBUG::Print() {
  fprintf(stderr, "Debuggings are working ... \n");
}

BOOL UWASM_DEBUG::Is_ofst_watched(UINT32 ofst) {
  return _watchpoints_cache.find(ofst) != _watchpoints_cache.end();
}
BOOL UWASM_DEBUG::Is_inst_breakable(AM_EXPAND *instr) {
  return _breakpoints_cache.find(instr) != _breakpoints_cache.end();
}

BOOL UWASM_DEBUG::Is_valid_breakpoint(AM_EXPAND *exp) {
  // Check if it's breakpoint of next command / valid breakpoint.
  return 1;
}

static UWASM_DEBUG *uvm_debug_tracker = NULL;
UWASM_DEBUG & Uvm_debug_tracker() {
  if (!uvm_debug_tracker) {
    uvm_debug_tracker = new UWASM_DEBUG();
  }
  return *uvm_debug_tracker;
}
