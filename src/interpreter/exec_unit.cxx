/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/8.
//
#include "u_types.h"
#include "ubh/ubh.h"
#include "exec_unit.h"
#include "runtime/exec_system.h"
#include "runtime/u_runtime.h"
#include "u_am.h"
#include "exec_unit_func.h"
#include "trace/exec_perf.h"
#include "exec_debug.h"

void EXEC_CONTEXT::Print(FILE *file) const {
  fprintf(file, "fp: I32(%d) I64(%d) F32(%d) F64(%d), Func idx: %d,  pc: %p, label tab: %p, sp: %p.\n",
    Fp_i32(), Fp_i64(), Fp_f32(), Fp_f64(),Get_func_idx(), Get_pc(), _label_tab, Get_sp());
}

void EXEC_CONTEXT::Print_detail(FILE *file, UWASM_MACHINE &machine) const {
  fprintf(file, "fp: I32(%d) I64(%d) F32(%d) F64(%d), func idx: %d, pc: %d, sp: %d.\n",
    Fp_i32(), Fp_i64(), Fp_f32(), Fp_f64(), Get_func_idx(),
    Get_pc() == nullptr ? 0 : machine.Get_u_module().Get_pc_idx(Get_pc()),
    machine._sta_p == nullptr ? 0 : (INT32) (Get_sp() - machine._sta_p));
}

void UWASM_MACHINE::Print(FILE *f) {
  fprintf(f, "%sMachine PC: %d(0x%04x) FuncIdx: %d \n"
             "Register Region Max: I32 : 0x%03x, I64 : 0x%03x, F32: 0x%03x, F64 : 0x%03x\n"
             "Register FP        : I32 : 0x%03x, I64 : 0x%03x, F32: 0x%03x, F64 : 0x%03x\n"
             "%s"
             "Register Content: \n",
          DBAR,
             Cur_env().Get_pc() == nullptr ? 0 : _module.Get_pc_idx(Cur_env().Get_pc()),
             Cur_env().Get_pc() == nullptr ? 0 : _module.Get_pc_idx(Cur_env().Get_pc()),
          Cur_env().Get_func_idx(),
          (UINT32) Reg_i32().Get_max(), (UINT32) Reg_i64().Get_max(),
          (UINT32) Reg_f32().Get_max(), (UINT32) Reg_f64().Get_max(),
          Reg_i32().Fp(), Reg_i64().Fp(),
          Reg_f32().Fp(), Reg_f64().Fp(),
          DBAR);
  for (UINT32 i = Reg_i32().Fp(); i < Reg_i32().Get_max(); i++) {
    UINT32 user_visible_index = i - Reg_i32().Fp();
    fprintf(f, "0x%08x ", Reg_i32()[user_visible_index]);
    if (user_visible_index % 4 == 3 || i == Reg_i32().Get_max() - 1) {
      fprintf(f, "\n");
    }
  }
}

UINT32 UWASM_MACHINE::Init_memory(char *mem_p, UINT32 mem_size) {
  _mem_p = mem_p;
  _mem_size = mem_size;
  UINT32 ret = _module.Prepare_before_run(_mem_p, _mem_size);
  if (ret != NUM_SUCC) {
    return ret;
  }
  if (mem_size == 0) {
    return ret;
  }
  _sta_p = _mem_p + mem_size;
  _hea_off = 0;
  if (_module.Contains_section(S_DATA)){
    _hea_off = _module.Get_shdr_by_sec_id(S_DATA).Get_elem_num();
  }
  if (_hea_off % PAGE_SIZE != 0) {
    _hea_off = (_hea_off / PAGE_SIZE + 1) * PAGE_SIZE;
  }
  Is_True(_hea_off <= mem_size,
          ("VM init memory is not enough, memory size must be page aligned, memory size: %d, "
           "minimum requirement: %d, page size: %d.", mem_size, _hea_off, PAGE_SIZE));
  _hea_p = _mem_p + _hea_off;
  _hea_size = (UINT32) ((float) (_mem_size - _hea_off) * HEAP_MEM_PERCENT);
  if (_hea_size % PAGE_SIZE != 0) {
    _hea_size = (_hea_size / PAGE_SIZE + 1) * PAGE_SIZE;
  }
  if (_module.Contains_section(S_GLBL)) {
    // Initialize global table.
    UINT32 glbl_cnt = _module.Get_shdr_by_sec_id(S_GLBL).Get_elem_num();
    _global_start = (GTABLE_ENTRY *) malloc(glbl_cnt * sizeof(GTABLE_ENTRY));
    UBH_GLBL *glbl = _module.Glbl_sec();
    for (UINT32 i = 0; i < glbl_cnt; i++) {
      _global_start[i] = _module.Get_glbl(i).Get_init_val();
    }
  }
  UVM_heap_initialize(_hea_p, _hea_size);
  return ret;
}

// execute
void UWASM_MACHINE::Exec_start() {
  Is_Trace(Tracing(TP_EXEC), (TFile, DBAR "Start executing:\n" DBAR));
  UWASM_PERF *_perf = &Uvm_perf_tracker();
  _perf->Init();
  UWASM_DEBUG *_debug = &Uvm_debug_tracker();
  _debug->Init(this);
  AM_EXPAND *prev_inst = nullptr;
  while(true) {
    Is_Trace_cmd(prev_inst != nullptr && Tracing(TP_EXEC), Print_result(prev_inst, TFile));
    AM_EXPAND *inst = Cur_env().Get_pc();
    prev_inst = inst;
    Is_Perf_cmd(Tracing(TP_EPERF), {
      _perf->Record_op(inst->Get_opcode_l());
    });
    Is_Trace_cmd(_debug->Get_bp_count() != 0, {
      if(_debug->Is_inst_breakable(inst)) {
        // Break it befoaaare this instruction is executed.
        if (_debug->Is_valid_breakpoint(inst)) {
          _debug->On_breakpoint(inst);
          return;
        }
      }
    });
    // Execute the instruction.
    switch(inst->Get_opcode_l()) {
      #include "exec_unit_op.def"
    }
    Cur_env().Inc_pc();
  }
exec_end:
  _perf->Set_ret_val(Get_ret_val());
  _perf->Print();
}

// run n instrs
void UWASM_MACHINE::Control_start(UINT64 instr_count) {
  AM_EXPAND *prev_inst = nullptr;
  UWASM_DEBUG *_debug = &Uvm_debug_tracker();
  _debug->Init(this);
  while(instr_count--) {
    // Before
    Is_Trace_cmd(Tracing(TP_EXEC), Cur_env().Get_pc()->Print(TFile));
    Is_Trace_cmd(prev_inst != nullptr && Tracing(TP_EXEC), Print_result(prev_inst, TFile));
    AM_EXPAND *inst = Cur_env().Get_pc();
    prev_inst = inst;
    switch(inst->Get_opcode_l()) {
      #include "exec_unit_op.def"
    }
    Is_Trace_cmd(_debug->Get_bp_count() != 0, {
      if(_debug->Is_inst_breakable(inst)) {
        // Break it befoaaare this instruction is executed.
        if (_debug->Is_valid_breakpoint(inst)) {
          _debug->On_breakpoint(inst);
          return;
        }
      }
    });
    Cur_env().Inc_pc();
  }
exec_end:
  // After
  Is_Trace_cmd(Tracing(TP_EXEC), Cur_env().Get_pc()->Print(TFile));
  return;
}

// Initialize the current exec env in the UWASM_SERVICE.
// this updates func_idx, pc, and fp(s), set fp to zero.
void UWASM_MACHINE::Init_env(UINT32 func_idx) {
  Is_Trace(Tracing(TP_EXEC), (TFile, "Init_env : func_idx = 0x%x\n", func_idx));
  // add entry frame, pc point to break instruction, thus vm can break from infinite loop
  Cur_env().Set_func_idx(func_idx);
  Cur_env().Set_pc(_module.Get_end_pc());
  // allocate new context frame, save init function's context
  _ctx_stack.Push(Cur_env());
  Set_func_idx(func_idx);
  // init current stack pointer
  Cur_env().Set_sp(_sta_p);
  // Update PC and function idx.
  // Allocate new register frame
  UINT64 reg_num = _module.Get_reg_num(func_idx);
  UINT16 *reg_num_arr = (UINT16 *) &reg_num;
  if (reg_num_arr[0] != 0) {
    Reg_i32().Alloc(reg_num_arr[0]);
  }
  if (reg_num_arr[1] != 0) {
    Reg_i64().Alloc(reg_num_arr[1]);
  }
  if (reg_num_arr[2] != 0) {
    Reg_f32().Alloc(reg_num_arr[2]);
  }
  if (reg_num_arr[3] != 0) {
    Reg_f64().Alloc(reg_num_arr[3]);
  }
  Allocate_stack(func_idx);
  Is_Trace(Tracing(TP_EXEC), (TFile, "Allocate i32 register: %d.\n", reg_num_arr[0]));
  Is_Trace(Tracing(TP_EXEC), (TFile, "Allocate i64 register: %d.\n", reg_num_arr[1]));
  Is_Trace(Tracing(TP_EXEC), (TFile, "Allocate f32 register: %d.\n", reg_num_arr[2]));
  Is_Trace(Tracing(TP_EXEC), (TFile, "Allocate f64 register: %d.\n", reg_num_arr[3]));
}

// Function prologue
void UWASM_MACHINE::Func_prologue(UINT32 callee_idx) {
  Is_Trace_cmd(Tracing(TP_EXEC), {
    char *caller_func_name = _module.Get_module_name(_module.Get_func(Cur_env().Get_func_idx()).Get_name_idx());
    char *callee_func_name = _module.Get_module_name(_module.Get_func(callee_idx).Get_name_idx());
    fprintf(TFile, ">>>> Call: %s -> %s\n", caller_func_name, callee_func_name);
  });
  UBH_FUNC &callee = _module.Get_func(callee_idx);
  // push current context to stack
  // create new context, save information
  UINT32 caller_func_idx = Cur_env().Get_func_idx();
  EXEC_CONTEXT &prev_ctx = _ctx_stack.Push(Cur_env());
  Cur_env().Set_func_idx(callee_idx);
  // Update frame pointer
  UINT64 fp_shs = _module.Get_fp_shs_num(caller_func_idx);
  if (fp_shs > 0xffffu) {
    UINT16 *fp_shs_arr = (UINT16 *) &fp_shs;
    Reg_i32().Increment(fp_shs_arr[0]);
    Cur_env().Set_fp_i32(Reg_i32().Fp());
    if (fp_shs_arr[1] != 0) {
      Reg_i64().Increment(fp_shs_arr[1]);
      Cur_env().Set_fp_i64(Reg_i64().Fp());
    }
    if (fp_shs_arr[2] != 0) {
      Reg_f32().Increment(fp_shs_arr[2]);
      Cur_env().Set_fp_f32(Reg_f32().Fp());
    }
    if (fp_shs_arr[3] != 0) {
      Reg_f64().Increment(fp_shs_arr[3]);
      Cur_env().Set_fp_f64(Reg_f64().Fp());
    }
  } else {
    Reg_i32().Increment(fp_shs);
    Cur_env().Set_fp_i32(Reg_i32().Fp());
  }
  // If external function. Call that function with function table.
  // and return directly, continue executing with correct PC.
  if (callee.Is_set_flag(F_EXTERN) || callee.Is_set_flag(F_NATIVE)) {
    U_RUNTIME::Call_sys_call(*this, callee_idx);
    Func_epilogue();
    Cur_env().Inc_pc();
  } else {
    Cur_env().Set_cur_label_tab(_module.Get_label_tab(callee_idx));
    Cur_env().Set_pc(_module.Get_expanded_func_base(callee_idx));
    Allocate_stack(callee_idx);
    UINT64 reg_num = Get_u_module().Get_reg_num(callee_idx);
    if (reg_num > 0xffffu) {
      UINT16 *reg_num_arr = (UINT16 *) &reg_num;
      Reg_i32().Alloc(reg_num_arr[0]);
      if (reg_num_arr[1] != 0) {
        Reg_i64().Alloc(reg_num_arr[1]);
      }
      if (reg_num_arr[2] != 0) {
        Reg_f32().Alloc(reg_num_arr[2]);
      }
      if (reg_num_arr[3] != 0) {
        Reg_f64().Alloc(reg_num_arr[3]);
      }
    } else {
      Reg_i32().Alloc(reg_num);
    }
  }
}

// Return the control to the caller, cleanup callee data on register.
void UWASM_MACHINE::Func_epilogue() {
  UINT32 prev_func_idx = Cur_env().Get_func_idx();
  EXEC_CONTEXT &prev_ctx = _ctx_stack.Top();
  // remove last context
  _current.Copy(prev_ctx);
  _ctx_stack.Pop();
  UINT64 fp_shs = Get_u_module().Get_fp_shs_num(_current.Get_func_idx());
  Reg_i32().Set_fp(Cur_env().Fp_i32());
  if (fp_shs > 0xffffu) {
    Reg_i64().Set_fp(Cur_env().Fp_i64());
    Reg_f32().Set_fp(Cur_env().Fp_f32());
    Reg_f64().Set_fp(Cur_env().Fp_f64());
  }
  Is_Trace_cmd(Tracing(TP_EXEC), {
    char *caller_func_name = _module.Get_module_name(_module.Get_func(Cur_env().Get_func_idx()).Get_name_idx());
    char *callee_func_name = _module.Get_module_name(_module.Get_func(prev_func_idx).Get_name_idx());
    fprintf(TFile, "<<<< Ret: %s <- %s\n", caller_func_name, callee_func_name);
  });
}

void UWASM_MACHINE::Print_reg(VALTYPE valtype, UINT32 idx, FILE *file) const {
  switch (valtype) {
    case I32:
      fprintf(file, "R%d[%d](%d)", idx, Reg_i32().Fp() + idx, Reg_i32().Get(idx));
      break;
    case I64:
      fprintf(file, "RD%d[%d](%lld)", idx, Reg_i64().Fp() + idx, Reg_i64().Get(idx));
      break;
    case F32:
      fprintf(file, "F%d[%d](%f)", idx, Reg_f32().Fp() + idx, Reg_f32().Get(idx));
      break;
    case F64:
      fprintf(file, "FD%d[%d](%lf)", idx, Reg_f64().Fp() + idx, Reg_f64().Get(idx));
      break;
  }
}

void UWASM_MACHINE::Print_reg_ofst_by_print_type(PRINT_TYPE pt, UINT32 idx, FILE *file) const {
  switch (pt) {
    case P_I32:
      fprintf(file, "R%d[%d](%d)", idx, Reg_i32().Fp() + idx, Reg_i32().Get(idx));
      break;
    case P_U32:
      fprintf(file, "R%d[%d](%uU)", idx, Reg_i32().Fp() + idx, (UINT32) Reg_i32().Get(idx));
      break;
    case P_I64:
      fprintf(file, "RD%d[%d](%lld)", idx, Reg_i64().Fp() + idx, Reg_i64().Get(idx));
      break;
    case P_U64:
      fprintf(file, "RD%d[%d](%lluU)", idx, Reg_i64().Fp() + idx, (UINT64) Reg_i64().Get(idx));
      break;
    case P_F32:
      fprintf(file, "F%d[%d](%f)", idx, Reg_f32().Fp() + idx, Reg_f32().Get(idx));
      break;
    case P_F64:
      fprintf(file, "FD%d[%d](%lf)", idx, Reg_f64().Fp() + idx, Reg_f64().Get(idx));
      break;
    case P_OFS:
#ifdef __M64__
      fprintf(file, "RD%d[%d](%ld)", idx, Reg_i64().Fp() + idx, Reg_i64().Get(idx));
#else
      fprintf(file, "R%d[%d](%d)", idx, Reg_i32().Fp() + idx, Reg_i32().Get(idx));
#endif
      break;
    default:
      break;
  }
}

U_OPCODE_LONG LOCAL_LDST_OPCODE_ARR[] = {
  LLOAD_I32_AM32_R_IMM_L,
  LLOAD_I64_AM32_R_IMM_L,
  LLOAD_F32_AM32_R_IMM_L,
  LLOAD_F64_AM32_R_IMM_L,
  LLOAD8_I32_AM32_R_IMM_L,
  LLOAD8_I64_AM32_R_IMM_L,
  LLOAD8_U_I32_AM32_R_IMM_L,
  LLOAD8_U_I64_AM32_R_IMM_L,
  LLOAD16_I32_AM32_R_IMM_L,
  LLOAD16_I64_AM32_R_IMM_L,
  LLOAD16_U_I32_AM32_R_IMM_L,
  LLOAD16_U_I64_AM32_R_IMM_L,
  LLOAD32_I64_AM32_R_IMM_L,
  LLOAD32_U_I64_AM32_R_IMM_L,
  LLOAD_I32_AM64_R_IMM_L,
  LLOAD_I64_AM64_R_IMM_L,
  LLOAD_F32_AM64_R_IMM_L,
  LLOAD_F64_AM64_R_IMM_L,
  LLOAD8_I32_AM64_R_IMM_L,
  LLOAD8_I64_AM64_R_IMM_L,
  LLOAD8_U_I32_AM64_R_IMM_L,
  LLOAD8_U_I64_AM64_R_IMM_L,
  LLOAD16_I32_AM64_R_IMM_L,
  LLOAD16_I64_AM64_R_IMM_L,
  LLOAD16_U_I32_AM64_R_IMM_L,
  LLOAD16_U_I64_AM64_R_IMM_L,
  LLOAD32_I64_AM64_R_IMM_L,
  LLOAD32_U_I64_AM64_R_IMM_L,
  LSTORE_I32_AM32_R_IMM_L,
  LSTORE_I64_AM32_R_IMM_L,
  LSTORE_F32_AM32_R_IMM_L,
  LSTORE_F64_AM32_R_IMM_L,
  LSTORE8_I32_AM32_R_IMM_L,
  LSTORE8_I64_AM32_R_IMM_L,
  LSTORE16_I32_AM32_R_IMM_L,
  LSTORE16_I64_AM32_R_IMM_L,
  LSTORE32_I64_AM32_R_IMM_L,
  LSTORE_I32_AM64_R_IMM_L,
  LSTORE_I64_AM64_R_IMM_L,
  LSTORE_F32_AM64_R_IMM_L,
  LSTORE_F64_AM64_R_IMM_L,
  LSTORE8_I32_AM64_R_IMM_L,
  LSTORE8_I64_AM64_R_IMM_L,
  LSTORE16_I32_AM64_R_IMM_L,
  LSTORE16_I64_AM64_R_IMM_L,
  LSTORE32_I64_AM64_R_IMM_L,
};

/**
 * Part of instructions have more than one operands, the value type of operands is not
 * defined by valty, the type information is encoded into the instruction
 * need to print them specially
 * @param inst
 * @param file
 * @return
 */
bool UWASM_MACHINE::Print_result_special(AM_EXPAND *inst, FILE *file) const {
  UINT32 opcode_l = inst->Get_opcode_l();
  INT32 s_idx = U_INST_UTIL::Get_special_format((U_OPCODE_LONG) opcode_l);
  if (s_idx == -1) {
    return false;
  }
  const TY_AM am_ty = UWINST_TY_AM[opcode_l >> VALTY_SZ];
  U_SPECIAL_FORMAT &sf = SPECIAL_FORMAT_ARR[s_idx];
  Print_reg_ofst_by_print_type(sf.pt_arr[0], inst->Get_rd(), file);
  fprintf(file, " ");
  Print_reg_ofst_by_print_type(sf.pt_arr[1], inst->Get_rs(), file);
  if (am_ty == TY_AM32S_R_R_R || am_ty == TY_AM64_R_R_R) {
    fprintf(file, " ");
    Print_reg_ofst_by_print_type(sf.pt_arr[2], inst->Get_rt(), file);
  } else if (am_ty == TY_AM32S_R_R_IMM || am_ty == TY_AM64_R_R_IMM) {
    fprintf(file, " ");
    fprintf(file, " (%d)", inst->Get_imm());
  }
  Is_Trace_cmd(true, Print_debug_info(inst, file));
  fprintf(file, "\n");
  return true;
}

void UWASM_MACHINE::Print_result(AM_EXPAND *inst, FILE *file, const char *slot1, const char *slot2) const {
  UINT32 opcode_l = inst->Get_opcode_l();
  const UINT8 opcode = (inst->Get_opcode_l() >> VALTY_SZ);
  const VALTYPE valtype = VALTYPE(inst->Get_opcode_l() & TYPE_MASK);
  fprintf(file, "%s", inst_impl_func_name[inst->Get_opcode_l()]);
  const TY_AM am_ty = UWINST_TY_AM[opcode];

  if (am_ty == TY_AM32_OFS) {
    fprintf(file, " (%d)", inst->Get_ofs());
    Is_Trace_cmd(true, Print_debug_info(inst, file));
    fprintf(file, "\n");
    return;
  } else if (am_ty == TY_AM32) {
    Is_Trace_cmd(true, Print_debug_info(inst, file));
    fprintf(file, "\n");
    return;
  }
  // type with at least an rd
  fprintf(file, " ");
  bool dump = Print_result_special(inst, file);
  if (dump) {
    return;
  }
  // all am32_r register type is i32
  if (am_ty == TY_AM32_R) {
    Print_reg(I32, inst->Get_rd(), file);
    Is_Trace_cmd(true, Print_debug_info(inst, file));
    fprintf(file, "\n");
    return;
  }
  // printf first operand
  Print_reg(valtype, inst->Get_rd(), file);
  // print second operand
  if (am_ty == TY_AM32_R_IMM || am_ty == TY_AM64_R_IMM) {
    bool is_local_ldst = false;
    for (UINT32 i = 0; i < sizeof(LOCAL_LDST_OPCODE_ARR) / sizeof(U_OPCODE_LONG); i++) {
      if (LOCAL_LDST_OPCODE_ARR[i] == opcode_l) {
        is_local_ldst = true;
        break;
      }
    }
    fprintf(file, "%s", slot1);
    if (is_local_ldst) {
      fprintf(file, " SP%d[%d]", inst->Get_imm(), (INT32) (Cur_env().Get_sp() + inst->Get_imm() - _sta_p));
    } else {
      fprintf(file, " (%d)", inst->Get_imm());
    }
    Is_Trace_cmd(true, Print_debug_info(inst, file));
    fprintf(file, "\n");
    return;
  } else if (am_ty == TY_AM32_R_OFS || am_ty == TY_AM64_R_OFS) {
    fprintf(file, "%s", slot1);
    fprintf(file, " (%d)", inst->Get_ofs());
    Is_Trace_cmd(true, Print_debug_info(inst, file));
    fprintf(file, "\n");
    return;
  }
  // type with at least two reg
  fprintf(file, "%s", slot1);
  fprintf(file, " ");
  // the type of address base was defined by memory module
  Print_reg(valtype, inst->Get_rs(), file);
  // print third content
  if (am_ty == TY_AM32S_R_R_IMM ||  am_ty == TY_AM64_R_R_IMM) {
    fprintf(file, "%s", slot2);
    fprintf(file, " (%d)", inst->Get_imm());
  } else if (am_ty == TY_AM32S_R_R_OFS ||  am_ty == TY_AM64_R_R_OFS) {
    fprintf(file, "%s", slot2);
    fprintf(file, " (%d)", inst->Get_ofs());
  } else if (am_ty == TY_AM32S_R_R_R || am_ty == TY_AM64_R_R_R) {
    fprintf(file, "%s ", slot2);
    Print_reg(valtype, inst->Get_rt(), file);
  }
  Is_Trace_cmd(true, Print_debug_info(inst, file));
  fprintf(file, "\n");
}

void UWASM_MACHINE::Print_debug_info(AM_EXPAND *inst, FILE *file) const {
  // Print debugging info
  UWASM_DEBUG &debug = Uvm_debug_tracker();
  UINT32 func_id;
  UINT32 uh_pc;
  _module.Map_to_ubh_pc(inst, func_id, uh_pc);
  fprintf(file, ", inst_pc : %d", uh_pc);
}

void UWASM_MACHINE::Print_result(AM_EXPAND *inst, FILE *file) const {
  Print_result(inst, file, "", "");
}

UINT32 UWASM_MACHINE::Grow_memory(UINT32 target_size_bytes) {
  Is_True(target_size_bytes >= _mem_size,
          ("Memory size cannot shrink. old = 0x%08x, new = 0x%08x",
            _mem_size, target_size_bytes));
  Is_True(target_size_bytes % MEM_PAGE_SIZE == 0,
          ("The page size should be aligned by mem-page , target = 0x%08x",
           target_size_bytes));
  if (target_size_bytes < _mem_size || target_size_bytes % MEM_PAGE_SIZE != 0) {
    return (UINT32) -1;
  }
  void *newptr = realloc(_mem_p, target_size_bytes);
  if (newptr == nullptr) {
    return (UINT32) -1;
  }
  _mem_size = target_size_bytes;
  return _mem_size;
}