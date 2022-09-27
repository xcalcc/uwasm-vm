/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_EXEC_UNIT_FUNC_H
#define UWASM_VM_EXEC_UNIT_FUNC_H

#include <cmath>

#define INLINE inline

// >>>>>>>> implemented code begin >>>>>>>>>>

INLINE void UWASM_MACHINE::Nop_am32(AM_EXPAND *inst) {
  (void) inst;
}

INLINE void UWASM_MACHINE::Unreach_am32(AM_EXPAND *inst) {
  Is_True(false, ("Unreachable instruction executed."));
  (void) inst;
}

INLINE void UWASM_MACHINE::Return_am32(AM_EXPAND *inst) {
  Func_epilogue();
}

INLINE void UWASM_MACHINE::Call_am32_ofs(AM_EXPAND *inst) {
  Func_prologue(inst->Get_ofs());
}

INLINE void UWASM_MACHINE::Jmp_am32_ofs(AM_EXPAND *inst) {
  Set_pc(Cur_env().Get_label(inst->Get_ofs()));
}

INLINE void UWASM_MACHINE::Call_indirect_am32_r(AM_EXPAND *inst) {
  inst->Set_ofs(_reg_i32[inst->Get_rd()]);
  Call_am32_ofs(inst);
}

INLINE void UWASM_MACHINE::Jmp_indirect_am32_r(AM_EXPAND *inst) {
  inst->Set_ofs(_reg_i32[inst->Get_rd()]);
  Jmp_am32_ofs(inst);
}

INLINE void UWASM_MACHINE::Add_i32_am32_r_imm(AM_EXPAND *inst) {
  Is_Trace(Tracing(TP_EXEC), (TFile, "[REG] R%d = %d + %d\n", inst->Get_rd(), _reg_i32[inst->Get_rd()], (INT32) inst->Get_imm()));
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rd()] + (INT32) inst->Get_imm());
}

INLINE void UWASM_MACHINE::Add_i64_am32_r_imm(AM_EXPAND *inst) {
  Is_Trace(Tracing(TP_EXEC), (TFile, "[REG] R%d = %lld + %lld\n", inst->Get_rd(), _reg_i64[inst->Get_rd()], (INT64) inst->Get_imm()));
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rd()] + (INT64) inst->Get_imm());
}

INLINE void UWASM_MACHINE::Jez_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] == 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jez_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] == 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jez_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] == 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jez_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] == 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jnz_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] != 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jnz_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] != 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jnz_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] != 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jnz_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] != 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlz_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] < 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlz_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] < 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlz_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] < 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlz_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] < 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlez_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] <= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlez_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] <= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlez_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] <= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlez_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] <= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgz_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] > 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgz_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] > 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgz_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] > 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgz_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] > 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgez_i32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] >= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgez_i64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] >= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgez_f32_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] >= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgez_f64_am32_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] >= 0) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

/*
 * Load local address
 * For current memory module, address is offset based on stack pointer
 * Local address offset is negative number, if want to load content,
 * just use stack pointer add offset, and then you can get the real offset
 * The design purpose is that we want to support reallocate shared memory later,
 * if reallocate memory, just copy all stack data to new content, the offset
 * is based on stack pointer (end of the memory), all load and store are normal
 */
INLINE void UWASM_MACHINE::Llda_i32_am32_r_ofs(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (INT32) Get_ofst_addr(inst->Get_ofs(), true));
}

INLINE void UWASM_MACHINE::Llda_i64_am32_r_ofs(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64) Get_ofst_addr(inst->Get_ofs(), true));
}

INLINE void UWASM_MACHINE::Glda_i32_am32_r_ofs(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (INT32) Get_ofst_addr(inst->Get_ofs(), false));
}

INLINE void UWASM_MACHINE::Glda_i64_am32_r_ofs(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64) Get_ofst_addr(inst->Get_ofs(), false));
}

INLINE void UWASM_MACHINE::Globalget_i32_am64_r_ofs(AM_EXPAND *inst) {
  UINT64 ptr = Get_global_addr(inst->Get_ofs());
  _reg_i32.Set(inst->Get_rd(), *((INT32 *) ptr));
}

INLINE void UWASM_MACHINE::Globalget_i64_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Globalget_f32_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Globalget_f64_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }

INLINE void UWASM_MACHINE::Globalset_i32_am64_r_ofs(AM_EXPAND *inst) {
  UINT64 ptr = Get_global_addr(inst->Get_ofs());
  *((INT32 *) ptr) = _reg_i32.Get(inst->Get_rd());
}

INLINE void UWASM_MACHINE::Globalset_i64_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Globalset_f32_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Globalset_f64_am64_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }

INLINE void UWASM_MACHINE::Mov_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Mov_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Mov_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Mov_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Add_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rd()] + _reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Add_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rd()] + _reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Add_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rd()] + _reg_f32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Add_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rd()] + _reg_f64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Const_i32_am32_r_imm(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), inst->Get_imm());
}

INLINE void UWASM_MACHINE::Const_i64_am32_r_imm(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64) ((INT32) inst->Get_imm()));
}

INLINE void UWASM_MACHINE::Add_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] + (INT32)inst->Get_imm());
}

INLINE void UWASM_MACHINE::Add_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] + (INT64)inst->Get_imm());
}

INLINE void UWASM_MACHINE::Jeq_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] == _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] == _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] == _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] == _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] != _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] != _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] != _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] != _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] < _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] < _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] < _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] < _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] <= _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] <= _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] <= _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] <= _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] > _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] > _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] > _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] > _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] >= _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] >= _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_f32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] >= _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_f64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] >= _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_u_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] < (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_u_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] < (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_u_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] <= (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_u_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] <= (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_u_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] > (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_u_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] > (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_u_i32_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] >= (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_u_i64_am32s_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] >= (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

/*
 * The following load and store instructions are offset load and store instructions
 * the semantic is similar with WHIRL ILOAD and ISTORE
 * load target_reg base_addr offset
 * store target_reg base_addr offset
 * base addr is not pointer, it is offset based on memory pointer
 */
INLINE void UWASM_MACHINE::Load_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Load_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT64 *) addr));
}

INLINE void UWASM_MACHINE::Load_f32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_f32.Set(inst->Get_rd(), *((FLOAT32 *) addr));
}

INLINE void UWASM_MACHINE::Load_f64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_f64.Set(inst->Get_rd(), *((FLOAT64 *) addr));
}

INLINE void UWASM_MACHINE::Load8_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_u_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_u_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Load16_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_u_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_u_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Load32_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Load32_u_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT32 *) addr));
}

INLINE void UWASM_MACHINE::Store_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT32 *) addr) = _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT64 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_f32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((FLOAT32 *) addr) = _reg_f32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_f64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((FLOAT64 *) addr) = _reg_f64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store8_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT8 *) addr) = (INT8) _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store8_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT8 *) addr) = (INT8) _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store16_i32_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT16 *) addr) = (INT16) _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store16_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT16 *) addr) = (INT16) _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store32_i64_am32s_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT32 *) addr) = (INT32) _reg_i64[inst->Get_rd()];
}

/*
 * The following load store instructions are local load and local store instruction
 * Just pick up the offset, load from/store to local stack
 * The semantic is similar with LOAD and STORE from WHIRL
 */
INLINE void UWASM_MACHINE::Lload_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Lload_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT64 *) addr));
}

INLINE void UWASM_MACHINE::Lload_f32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_f32.Set(inst->Get_rd(), *((FLOAT32 *) addr));
}

INLINE void UWASM_MACHINE::Lload_f64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_f64.Set(inst->Get_rd(), *((FLOAT64 *) addr));
}

INLINE void UWASM_MACHINE::Lload8_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Lload8_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Lload8_u_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Lload8_u_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Lload16_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Lload16_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Lload16_u_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Lload16_u_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Lload32_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Lload32_u_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT32 *) addr));
}

INLINE void UWASM_MACHINE::Lstore_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT32 *) addr) = _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT64 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore_f32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((FLOAT32 *) addr) = _reg_f32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore_f64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((FLOAT64 *) addr) = _reg_f64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore8_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT8 *) addr) = _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore8_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT8 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore16_i32_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT16 *) addr) = _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore16_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT16 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lstore32_i64_am32_r_imm(AM_EXPAND *inst) {
  char *addr = Cur_env().Get_sp() + inst->Get_imm();
  *((INT32 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Add_i32_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] + _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_i64_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] + _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_f32_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] + _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_f64_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] + _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_i32_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] - _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_i64_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] - _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_f32_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] - _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_f64_am32s_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] - _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Rotl_i32_am32s_r_r_r(AM_EXPAND *inst) {
  // rotate left
  UINT32 num_bit = _reg_i32[inst->Get_rt()];
  _reg_i32.Set(inst->Get_rd(), (num_bit << num_bit) | (UINT32)(num_bit >> (32 - num_bit)));
}

INLINE void UWASM_MACHINE::Rotl_i64_am32s_r_r_r(AM_EXPAND *inst) {
  UINT32 num_bit = _reg_i32[inst->Get_rt()];
  _reg_i64.Set(inst->Get_rd(), (num_bit << num_bit) | (UINT64)(num_bit >> (64 - num_bit)));
}

INLINE void UWASM_MACHINE::Rotr_i32_am32s_r_r_r(AM_EXPAND *inst) {
  // rotate right
  UINT32 num_bit = _reg_i32[inst->Get_rt()];
  _reg_i32.Set(inst->Get_rd(), (UINT32)(num_bit >> num_bit) | (num_bit << (32 - num_bit)));
}

INLINE void UWASM_MACHINE::Rotr_i64_am32s_r_r_r(AM_EXPAND *inst) {
  UINT32 num_bit = _reg_i32[inst->Get_rt()];
  _reg_i64.Set(inst->Get_rd(), (UINT64)(num_bit >> num_bit) | (num_bit << (64 - num_bit)));
}

INLINE void UWASM_MACHINE::Conv_i32_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_i32_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_i32_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_i32_u_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64) ((UINT32) _reg_i32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Conv_i64_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (INT32)_reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_i64_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_i64_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f32_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f32_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f32_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f64_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f64_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Conv_f64_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Eqz_i32_am32_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] == 0);
}

INLINE void UWASM_MACHINE::Eqz_i64_am32_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] == 0);
}

INLINE void UWASM_MACHINE::Eqz_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] == 0);
}

INLINE void UWASM_MACHINE::Eqz_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] == 0);
}

INLINE void UWASM_MACHINE::Ceil_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), ceil(_reg_f32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Ceil_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), ceil(_reg_f64[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Floor_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), floor(_reg_f32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Floor_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), floor(_reg_f64[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Sqrt_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), sqrt(inst->Get_rs()));
}

INLINE void UWASM_MACHINE::Sqrt_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), sqrt(inst->Get_rs()));
}

INLINE void UWASM_MACHINE::Neg_f32_am32_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), -_reg_i32[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Neg_f64_am32_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), -_reg_i64[inst->Get_rs()]);
}

INLINE void UWASM_MACHINE::Reintr_f32_i32_am32_r_r(AM_EXPAND *inst) {
  // reinterpret f32 to i32
  float value = _reg_f32.Get(inst->Get_rs());
  _reg_i32.Set(inst->Get_rd(), *((UINT32*) &value));
}

INLINE void UWASM_MACHINE::Reintr_f64_i64_am32_r_r(AM_EXPAND *inst) {
  double value = _reg_f64.Get(inst->Get_rs());
  _reg_i64.Set(inst->Get_rd(), *((UINT64 *) &value));
}

INLINE void UWASM_MACHINE::Reintr_i32_f32_am32_r_r(AM_EXPAND *inst) {
  INT32 value = _reg_i32.Get(inst->Get_rs());
  _reg_f32.Set(inst->Get_rd(), *((FLOAT32 *) &value));
}

INLINE void UWASM_MACHINE::Reintr_i64_f64_am32_r_r(AM_EXPAND *inst) {
  INT64 value = _reg_i64.Get(inst->Get_rs());
  _reg_f64.Set(inst->Get_rd(), *((FLOAT64 *) &value));
}

INLINE void UWASM_MACHINE::Break_am32(AM_EXPAND *inst) {
}

INLINE void UWASM_MACHINE::Const_l_i32_am64_r_imm(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), inst->Get_imm());
}

INLINE void UWASM_MACHINE::Const_l_i64_am64_r_imm(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64) inst->Get_imm());
}

INLINE void UWASM_MACHINE::Const_l_f32_am64_r_imm(AM_EXPAND *inst) {
  UINT32 value = inst->Get_ofs();
  _reg_f32.Set(inst->Get_rd(), reinterpret_cast<FLOAT32 &>(value));
}

INLINE void UWASM_MACHINE::Const_l_f64_am64_r_imm(AM_EXPAND *inst) {
  FLOAT64 orig_val = _reg_f64.Get(inst->Get_rd());
  UINT64 val = reinterpret_cast<UINT64 &>(orig_val);
  UINT64 const_imm = inst->Get_ofs();
  val = val & 0xFFFFFFFF00000000ll;
  val = val | const_imm;
  _reg_f64.Set(inst->Get_rd(), reinterpret_cast<FLOAT64 &>(val));
}

INLINE void UWASM_MACHINE::Const_l_high_i64_am64_r_imm(AM_EXPAND *inst) {
  INT64 val = _reg_i64.Get(inst->Get_rd());
  UINT64 const_imm = inst->Get_ofs();
  val = val & 0xFFFFFFFFll;
  val = val | (const_imm << 32ull);
  _reg_i64.Set(inst->Get_rd(), val);
}

INLINE void UWASM_MACHINE::Const_l_high_f64_am64_r_imm(AM_EXPAND *inst) {
  FLOAT64 orig_val = _reg_f64.Get(inst->Get_rd());
  UINT64 val = reinterpret_cast<UINT64 &>(orig_val);
  UINT64 const_imm = inst->Get_ofs();
  val = val & 0xFFFFFFFFul;
  val = val | (const_imm << 32u);
  _reg_f64.Set(inst->Get_rd(), reinterpret_cast<FLOAT64 &>(val));
}

//INLINE void UWASM_MACHINE::Clz_i32_am64_r_ofs(AM_EXPAND *inst) {
//  // count leading zero
//  UINT32 non_zero = 0;
//  while (inst->Get_ofs() >> non_zero) non_zero++;
//  _reg_i32.Set(inst->Get_rd(), 32 - non_zero);
//}
//
//INLINE void UWASM_MACHINE::Clz_i64_am64_r_ofs(AM_EXPAND *inst) {
//  UINT64 non_zero = 0;
//  while (inst->Get_ofs() >> non_zero) non_zero++;
//  _reg_i64.Set(inst->Get_rd(), 64 - non_zero);
//}

INLINE void UWASM_MACHINE::Ctz_i32_am64_r_ofs(AM_EXPAND *inst) {
  // count trailing zero
  UINT32 non_zero = 0;
  while (inst->Get_ofs() << non_zero) non_zero++;
  _reg_i32.Set(inst->Get_rd(), 32 - non_zero);
}

INLINE void UWASM_MACHINE::Ctz_i64_am64_r_ofs(AM_EXPAND *inst) {
  UINT64 non_zero = 0;
  while (inst->Get_ofs() << non_zero) non_zero++;
  _reg_i64.Set(inst->Get_rd(), 64 - non_zero);
}

INLINE void UWASM_MACHINE::Popcnt_i32_am64_r_ofs(AM_EXPAND *inst) {
  // count one in number
  UINT32 one_bit = 0;
  for (UINT32 i = 0; i < 32; i++) one_bit += (((UINT32)inst->Get_ofs() >> i) & 1);
  _reg_i32.Set(inst->Get_rd(), one_bit);
}

INLINE void UWASM_MACHINE::Popcnt_i64_am64_r_ofs(AM_EXPAND *inst) {
  UINT64 one_bit = 0;
  for (UINT64 i = 0; i < 64; i++) one_bit += (((UINT64)inst->Get_ofs() >> i) & 1);
  _reg_i64.Set(inst->Get_rd(), one_bit);
}

INLINE void UWASM_MACHINE::Add_i32_am64_r_r_imm(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] + (INT32)inst->Get_imm());
}

INLINE void UWASM_MACHINE::Add_i64_am64_r_r_imm(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] + (INT64)inst->Get_imm());
}

INLINE void UWASM_MACHINE::Jeq_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] == _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] == _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] == _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jeq_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] == _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] != _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] != _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] != _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jne_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] != _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] < _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] < _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] < _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] < _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] <= _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] <= _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] <= _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] <= _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] > _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] > _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] > _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] > _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i32[inst->Get_rd()] >= _reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_i64[inst->Get_rd()] >= _reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_f32_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f32[inst->Get_rd()] >= _reg_f32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_f64_am64_r_r_ofs(AM_EXPAND *inst) {
  if (_reg_f64[inst->Get_rd()] >= _reg_f64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_u_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] < (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jlt_u_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] < (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_u_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] <= (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jle_u_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] <= (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_u_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] > (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jgt_u_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] > (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_u_i32_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT32)_reg_i32[inst->Get_rd()] >= (UINT32)_reg_i32[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Jge_u_i64_am64_r_r_ofs(AM_EXPAND *inst) {
  if ((UINT64)_reg_i64[inst->Get_rd()] >= (UINT64)_reg_i64[inst->Get_rs()]) {
    Jmp_am32_ofs(inst);
  } else Cur_env().Inc_pc();
}

INLINE void UWASM_MACHINE::Load_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Load_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT64 *) addr));
}

INLINE void UWASM_MACHINE::Load_f32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_f32.Set(inst->Get_rd(), *((FLOAT32 *) addr));
}

INLINE void UWASM_MACHINE::Load_f64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_f64.Set(inst->Get_rd(), *((FLOAT64 *) addr));
}

INLINE void UWASM_MACHINE::Load8_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_u_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Load8_u_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT8 *) addr));
}

INLINE void UWASM_MACHINE::Load16_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_u_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i32.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Load16_u_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT16 *) addr));
}

INLINE void UWASM_MACHINE::Load32_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((INT32 *) addr));
}

INLINE void UWASM_MACHINE::Load32_u_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  _reg_i64.Set(inst->Get_rd(), *((UINT32 *) addr));
}

INLINE void UWASM_MACHINE::Store_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT32 *) addr) = _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT64 *) addr) = _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_f32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((FLOAT32 *) addr) = _reg_f32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store_f64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((FLOAT64 *) addr) = _reg_f64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store8_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT8 *) addr) = (INT8) _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store8_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT8 *) addr) = (INT8) _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store16_i32_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT16 *) addr) = (INT16) _reg_i32[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store16_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT16 *) addr) = (INT16) _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Store32_i64_am64_r_r_imm(AM_EXPAND *inst) {
  char *base_addr = Get_base_addr(inst->Get_rs());
  char *addr = base_addr + inst->Get_imm();
  *((INT32 *) addr) = (INT32) _reg_i64[inst->Get_rd()];
}

INLINE void UWASM_MACHINE::Lload_i32_am64_r_imm(AM_EXPAND *inst) {
  Lload_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload_f32_am64_r_imm(AM_EXPAND *inst) {
  Lload_f32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload_f64_am64_r_imm(AM_EXPAND *inst) {
  Lload_f64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload8_i32_am64_r_imm(AM_EXPAND *inst) {
  Lload8_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload8_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload8_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload8_u_i32_am64_r_imm(AM_EXPAND *inst) {
  Lload8_u_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload8_u_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload8_u_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload16_i32_am64_r_imm(AM_EXPAND *inst) {
  Lload16_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload16_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload16_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload16_u_i32_am64_r_imm(AM_EXPAND *inst) {
  Lload16_u_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload16_u_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload16_u_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload32_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload32_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lload32_u_i64_am64_r_imm(AM_EXPAND *inst) {
  Lload32_u_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore_i32_am64_r_imm(AM_EXPAND *inst) {
  Lstore_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore_i64_am64_r_imm(AM_EXPAND *inst) {
  Lstore_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore_f32_am64_r_imm(AM_EXPAND *inst) {
  Lstore_f32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore_f64_am64_r_imm(AM_EXPAND *inst) {
  Lstore_f64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore8_i32_am64_r_imm(AM_EXPAND *inst) {
  Lstore8_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore8_i64_am64_r_imm(AM_EXPAND *inst) {
  Lstore8_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore16_i32_am64_r_imm(AM_EXPAND *inst) {
  Lstore16_i32_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore16_i64_am64_r_imm(AM_EXPAND *inst) {
  Lstore16_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Lstore32_i64_am64_r_imm(AM_EXPAND *inst) {
  Lstore32_i64_am32_r_imm(inst);
}

INLINE void UWASM_MACHINE::Eq_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] == _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Eq_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] == _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Eq_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] == _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Eq_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] == _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ne_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] != _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ne_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] != _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ne_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] != _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ne_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] != _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] < _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] < _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] < _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] < _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32)_reg_i32[inst->Get_rs()] < (UINT32)_reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Lt_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64)_reg_i64[inst->Get_rs()] < (UINT64)_reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] > _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] > _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] > _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] > _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32)_reg_i32[inst->Get_rs()] > (UINT32)_reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Gt_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64)_reg_i64[inst->Get_rs()] > (UINT64)_reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] <= _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] <= _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] <= _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] <= _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32)_reg_i32[inst->Get_rs()] <= (UINT32)_reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Le_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64)_reg_i64[inst->Get_rs()] <= (UINT64)_reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] >= _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] >= _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] >= _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] >= _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32)_reg_i32[inst->Get_rs()] >= (UINT32)_reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Ge_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64)_reg_i64[inst->Get_rs()] >= (UINT64)_reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] + _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] + _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] + _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Add_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] + _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] - _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] - _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] - _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Sub_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] - _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Mul_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] * _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Mul_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] * _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Mul_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] * _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Mul_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] * _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Div_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] / _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Div_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] / _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Div_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), _reg_f32[inst->Get_rs()] / _reg_f32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Div_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), _reg_f64[inst->Get_rs()] / _reg_f64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Div_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32)(_reg_i32[inst->Get_rs()] / _reg_i32[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Div_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64)(_reg_i64[inst->Get_rs()] / _reg_i64[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Rem_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (INT32)_reg_i32[inst->Get_rs()] % (INT32)_reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Rem_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (INT64)_reg_i64[inst->Get_rs()] % (INT64)_reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Rem_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] % _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Rem_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] % _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::And_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] & _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::And_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] & _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Or_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] | _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Or_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] | _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Xor_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] ^ _reg_i32[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Xor_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] ^ _reg_i64[inst->Get_rt()]);
}

INLINE void UWASM_MACHINE::Shl_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] << ((UINT32) _reg_i32[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Shl_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] << ((UINT32) _reg_i32[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Shr_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), _reg_i32[inst->Get_rs()] >> ((UINT32) _reg_i32[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Shr_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), _reg_i64[inst->Get_rs()] >> ((UINT32) _reg_i32[inst->Get_rt()]));
}

INLINE void UWASM_MACHINE::Shr_u_i32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i32.Set(inst->Get_rd(), (UINT32) (((UINT32) _reg_i32[inst->Get_rs()]) >> ((UINT32) _reg_i32[inst->Get_rt()])));
}

INLINE void UWASM_MACHINE::Shr_u_i64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_i64.Set(inst->Get_rd(), (UINT64) (((UINT64) _reg_i64[inst->Get_rs()]) >> ((UINT32) _reg_i32[inst->Get_rt()])));
}

INLINE void UWASM_MACHINE::Min_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), fmin(_reg_f32[inst->Get_rs()], _reg_i32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Min_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), fmin(_reg_f64[inst->Get_rs()], _reg_i32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Max_f32_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f32.Set(inst->Get_rd(), fmax(_reg_f32[inst->Get_rs()], _reg_i32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Max_f64_am64_r_r_r(AM_EXPAND *inst) {
  _reg_f64.Set(inst->Get_rd(), fmax(_reg_f64[inst->Get_rs()], _reg_i32[inst->Get_rs()]));
}

INLINE void UWASM_MACHINE::Llda_i32_am64_r_ofs(AM_EXPAND *inst) {
  Llda_i32_am32_r_ofs(inst);
}

INLINE void UWASM_MACHINE::Llda_i64_am64_r_ofs(AM_EXPAND *inst) {
  Llda_i32_am64_r_ofs(inst);
}

INLINE void UWASM_MACHINE::Builtin_i32_am64_r_r_imm(AM_EXPAND *inst) {
  UINT16 builtin_code = inst->Get_imm();
  switch ((U_BUILTIN_CODE) builtin_code) {
    case U_BUILTIN_CODE::UBT_I32WRAPI64: {
      _reg_i32.Set(inst->Get_rd(), (INT32) _reg_i64.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_I32TRUNCF64U: {
      _reg_i32.Set(inst->Get_rd(), (UINT32) _reg_f64.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_I32TRUNCF64S: {
      _reg_i32.Set(inst->Get_rd(), (INT32) _reg_f64.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_I32REINTEPRETF32: {
      FLOAT32 n = _reg_f32.Get(inst->Get_rs());
      INT32   n2 = *(reinterpret_cast<INT32 *>(&n)); // returns zero, for all n1
      _reg_i32.Set(inst->Get_rd(), n2);
      break;
    }
    default: {
      Is_True(false, ("NYI, builtin opcode not impl = %d, %s",
        builtin_code,
        (builtin_code < U_BUILTIN_CODE::MAXBUILTIN_CODE ?
         U_BUILTIN_CODE_NAME[builtin_code] : "out-of-bound")));
    }
  }
}

INLINE void UWASM_MACHINE::Builtin_i64_am64_r_r_imm(AM_EXPAND *inst) {
  UINT16 builtin_code = inst->Get_imm();
  switch ((U_BUILTIN_CODE) builtin_code) {
    case U_BUILTIN_CODE::UBT_I64EXTEND32S: {
      _reg_i64.Set(inst->Get_rd(), (INT64) _reg_i32.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_I64EXTEND32U: {
      _reg_i64.Set(inst->Get_rd(), (UINT32)_reg_i32.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_I64REINTEPRETF64: {
      FLOAT64 n = _reg_f64.Get(inst->Get_rs());
      INT64   n2 = *(reinterpret_cast<INT64 *>(&n)); // returns zero, for all n1
      _reg_i64.Set(inst->Get_rd(), n2);
      break;
    }
    default: {
      Is_True(false, ("NYI, builtin opcode not impl = %d, %s",
       builtin_code,
      (builtin_code < U_BUILTIN_CODE::MAXBUILTIN_CODE ?
        U_BUILTIN_CODE_NAME[builtin_code] : "out-of-bound")));
    }
  }
}

INLINE void UWASM_MACHINE::Builtin_f32_am64_r_r_imm(AM_EXPAND *inst) {
  UINT16 builtin_code = inst->Get_imm();
  switch ((U_BUILTIN_CODE) builtin_code) {
    case U_BUILTIN_CODE::UBT_F32REINTEPRETI32: {
      INT32    n   = _reg_i32.Get(inst->Get_rs());
      FLOAT32  n2  = *(reinterpret_cast<FLOAT32 *>(&n)); // returns zero, for all n1
      _reg_f32.Set(inst->Get_rd(), n2);
      break;
    }
    default: {
      Is_True(false, ("NYI, builtin opcode not impl = %d, %s",
        builtin_code,
        (builtin_code < U_BUILTIN_CODE::MAXBUILTIN_CODE ?
         U_BUILTIN_CODE_NAME[builtin_code] : "out-of-bound")));
    }
  }
}

INLINE void UWASM_MACHINE::Builtin_f64_am64_r_r_imm(AM_EXPAND *inst) {
  UINT16 builtin_code = inst->Get_imm();
  switch ((U_BUILTIN_CODE) builtin_code) {
    case U_BUILTIN_CODE::UBT_F64CVTI32S: {
      _reg_f64.Set(inst->Get_rd(), (FLOAT64) _reg_i32.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_F64CVTI32U: {
      _reg_f64.Set(inst->Get_rd(), (FLOAT64) (UINT32) _reg_i32.Get(inst->Get_rs()));
      break;
    }
    case U_BUILTIN_CODE::UBT_F64REINTEPRETI64: {
      INT64    n   = _reg_i64.Get(inst->Get_rs());
      FLOAT64  n2  = *(reinterpret_cast<FLOAT64 *>(&n)); // returns zero, for all n1
      _reg_f64.Set(inst->Get_rd(), n2);
      break;
    }
    case U_BUILTIN_CODE::UBT_F64ABS: {
      FLOAT64  n   = _reg_f64.Get(inst->Get_rs());
      _reg_f64.Set(inst->Get_rd(), (double) fabs((double) n));
      break;
    }
    default: {
      Is_True(false, ("NYI, builtin opcode not impl = %d, %s",
        builtin_code,
        (builtin_code < U_BUILTIN_CODE::MAXBUILTIN_CODE ?
         U_BUILTIN_CODE_NAME[builtin_code] : "out-of-bound")));
    }
  }
}


INLINE void UWASM_MACHINE::Memorysize_am32_r(AM_EXPAND *inst) {
  Is_True(Mem_size() % MEM_PAGE_SIZE == 0, ("Invalid memory size"));
  Reg_i32().Set(inst->Get_rd(), Mem_size() / MEM_PAGE_SIZE);
}

INLINE void UWASM_MACHINE::Memorygrow_am32_r_r(AM_EXPAND *inst) {
  Is_True(Mem_size() % MEM_PAGE_SIZE == 0, ("Invalid memory size"));
  UINT32 pages_cnt = Reg_i32().Get(inst->Get_rs());
  Is_True(pages_cnt < UINT32_MAX / (UINT32) MEM_PAGE_SIZE,
          ("Pages size out of bound, pages_cnt = %u", pages_cnt));
  pages_cnt += Mem_size() / MEM_PAGE_SIZE;
  Is_True(pages_cnt < UINT32_MAX / (UINT32) MEM_PAGE_SIZE,
          ("Pages size combined out of bound, pages_cnt = %u", pages_cnt));
  UINT32 result = Grow_memory(pages_cnt * MEM_PAGE_SIZE);
  Reg_i32().Set(inst->Get_rd(), result);
}


// <<<<<<<<<< implemented code end <<<<<<<<<<

// >>>>>>>> generated code begin >>>>>>>>>>
INLINE void UWASM_MACHINE::Select_i32_am64_r_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Select_i64_am64_r_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Select_f32_am64_r_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
INLINE void UWASM_MACHINE::Select_f64_am64_r_r_ofs(AM_EXPAND *inst) { Is_True(false, (UIMPL)); }
// <<<<<<<<<< generated code end <<<<<<<<<<

#endif //UWASM_VM_EXEC_UNIT_FUNC_H
