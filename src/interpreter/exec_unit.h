/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_EXEC_UNIT_H
#define UWASM_VM_EXEC_UNIT_H

#include "ubh/ubh.h"
#include "ubh/u_module.h"
#include "uwasm_malloc.h"
#include "ubh/u_inst.h"
#include <map>
#include <vector>
#include <stdlib.h>

#define DEFAULT_STACK_SIZE 1024
#define MEMORY_DEFAULT_SIZE (64 * 1024)
#define INITIAL_REG_SIZE   (16 * 1024)
#define HEAP_MEM_PERCENT 0.8f
#define PAGE_SIZE 4096
#define MEM_PAGE_SIZE 65536

#ifdef __M64__
#define REGADDR _reg_i64
#define TY_ADDR I64
#else
#define REGADDR _reg_i32
#define TY_ADDR I32
#endif

typedef UINT64 GTABLE_ENTRY; // Each entry in the global table takes 8 bytes.
typedef vector<string> STRVEC;

class UWASM_MACHINE;
class UWASM_SYSTEM;

class EXEC_CONTEXT {
private:
  UINT32            _fp_i32;
  UINT32            _fp_i64;
  UINT32            _fp_f32;
  UINT32            _fp_f64;
  UINT32            _func_idx;     // current function index
  AM_EXPAND        *_pc;           // point to current instruction
  AM_EXPAND       **_label_tab;    // point to current label table
  char             *_sp;           // stack pointer

public:
  EXEC_CONTEXT() :_fp_i32(0), _fp_i64(0), _fp_f32(0), _fp_f64(0),
    _func_idx(0), _pc(nullptr), _label_tab(nullptr), _sp(nullptr) {}
  AM_EXPAND *Get_pc()                                 const { return _pc; }
  AM_EXPAND *Get_label(UINT32 lab_idx)                const { return _label_tab[lab_idx]; }
  UINT32     Get_func_idx()                           const { return _func_idx; }
  UINT32     Fp_i32()                                 const { return _fp_i32; }
  UINT32     Fp_i64()                                 const { return _fp_i64; }
  UINT32     Fp_f32()                                 const { return _fp_f32; }
  UINT32     Fp_f64()                                 const { return _fp_f64; }
  char *Get_sp()                                      const { return _sp; }

  void Set_pc(AM_EXPAND *pc)                                { _pc = pc; }
  void Set_func_idx(UINT32 func_idx)                        { _func_idx = func_idx; }
  void Set_cur_label_tab(AM_EXPAND **label_tab)             { _label_tab = label_tab; }
  void Set_fp_i32(UINT32 fp)                                { _fp_i32 = fp; }
  void Set_fp_i64(UINT32 fp)                                { _fp_i64 = fp; }
  void Set_fp_f32(UINT32 fp)                                { _fp_f32 = fp; }
  void Set_fp_f64(UINT32 fp)                                { _fp_f64 = fp; }
  void Inc_pc()                                             { _pc++; }
  void Set_sp(char *sp)                                     { _sp = sp; }
  void Copy(const EXEC_CONTEXT &context)                    { memcpy(this, &context, sizeof(EXEC_CONTEXT)); }
  void Print(FILE *file) const;
  void Print_detail(FILE *file, UWASM_MACHINE &machine) const;
};

template<typename REG_VALTY>
class REGISTER {
private:
  UINT32            _fp;                        // frame pointer
  UINT32            _cur_mem_size;
  REG_VALTY        *_register_begin; // a pointer into the data region in register.
  REG_VALTY        *_register_cur;   // a pointer into the data region in current register list.

public:
  REGISTER() : _fp(0) {
    _cur_mem_size = INITIAL_REG_SIZE;
    _register_begin = (REG_VALTY *) malloc(_cur_mem_size * sizeof(REG_VALTY));
    memset(_register_begin, 0, _cur_mem_size * sizeof(REG_VALTY));
    _register_cur = _register_begin;
  }
  REG_VALTY  operator[] (UINT32 i)             { return *(_register_cur + i);  }
  UINT32     Get_max()  const                  { return _fp + 50; /* TODO: FIXME: This is only for tracing purpose. */ }

  void Set(UINT32 i, REG_VALTY value)          { *(_register_cur + i) = value; }
  void Alloc(UINT32 len) {
    if (_cur_mem_size < _fp + len) {
      _cur_mem_size =  (_cur_mem_size + _fp + len) * 2;
      _register_begin = (REG_VALTY *) realloc(_register_begin, _cur_mem_size * sizeof(REG_VALTY));
      _register_cur = _register_begin + _fp;
    }
  }
  void Set_fp(UINT32 fp) {
    _fp = fp;
    _register_cur = _register_begin + fp;
  }
  void Increment(UINT32 size) {
    _fp += size;
    _register_cur = _register_begin + _fp;
  }
  REG_VALTY Get(UINT32 i)                   const { return *(_register_cur + i); }
  REG_VALTY *Get_addr(UINT32 i)             const { return _register_cur + i; }
  UINT32    Fp()                            const { return _fp; }
};

class CONTEXT_STACK {
  friend class UWASM_DEBUG;
private:
  vector<EXEC_CONTEXT>        _ctx_sta;
  INT32                       _capacity;
  INT32                       _sta_top_idx;

public:
  CONTEXT_STACK() :_sta_top_idx(-1)          { _ctx_sta.resize(DEFAULT_STACK_SIZE); _capacity = _ctx_sta.capacity(); }
  // this function just allocate a context, the return context should be initialized by caller
  EXEC_CONTEXT& Push(EXEC_CONTEXT &context) {
    // no space lefted, allocate new space
    if (_sta_top_idx == _capacity) {
      _capacity *= 2;
      _ctx_sta.resize(_capacity);
    }
    EXEC_CONTEXT &top = _ctx_sta[++_sta_top_idx];
    top.Copy(context);
    return top;
  }
  // we pushed a entry frame into the stack, so the top index will always be greater than 0
  EXEC_CONTEXT& Pop() {
    Is_True(_sta_top_idx >= 0, ("Stack top idx less than 0, can't pop, stack is not balance."));
    return _ctx_sta[--_sta_top_idx];
  }
  EXEC_CONTEXT& Top()                         { return _ctx_sta[_sta_top_idx]; }
protected:
  UINT32        Capacity()                    { return _capacity;              }
  UINT32        TopIdx()                      { return _sta_top_idx;           }
  vector<EXEC_CONTEXT>  &Vector()             { return _ctx_sta;               }
};

/**
 * The data structure to represent the whole WASM Execution unit.
 *
 * all memory vm used, was from out side, mush be initialize memory before run
 * memory are liner, contains 3 separations: global data, heap, stack
 * memory arrangement:
 * [low addr] { global data | heap -> (free space) <- stack } [high addr]
 *
 */
class UWASM_MACHINE {
  friend class UWASM_SYSTEM;
  friend class EXEC_CONTEXT;
  friend class U_RUNTIME;
  friend class UWASM_DEBUG;
  friend void UVM_set_reg_i32(uint32_t reg, int32_t v);
  friend void UVM_set_reg_i64(uint32_t reg, int64_t v);
  friend void UVM_set_reg_f32(uint32_t reg, float v);
  friend void UVM_set_reg_f64(uint32_t reg, double v);
  friend uint32_t UVM_get_reg_i32(uint32_t reg);
  friend uint64_t UVM_get_reg_i64(uint32_t reg);
  friend float UVM_get_reg_f32(uint32_t reg);
  friend double UVM_get_reg_f64(uint32_t reg);

private:
  REGISTER<INT32>               _reg_i32;
  REGISTER<INT64>               _reg_i64;
  REGISTER<FLOAT32>             _reg_f32;
  REGISTER<FLOAT64>             _reg_f64;
  CONTEXT_STACK                 _ctx_stack;
  U_MODULE                      _module;             // module has functions, instructions, etc.
  char                         *_mem_p;              // memory pointer, contains global data, stack memory, heap memory
  char                         *_sta_p;              // stack space pointer
  char                         *_hea_p;              // heap space pointer
  UINT32                        _mem_size;           // memory length
  UINT32                        _hea_off;            // heap pointer offset based on memory pointer
  UINT32                        _hea_size;
  GTABLE_ENTRY                 *_global_start;       // points to the begin of global table's value region
  EXEC_CONTEXT                  _current;
  UWASM_SYSTEM                 *_system;
  STRVEC                        _str_args; // wasi compatible start

private:
  void Set_func_idx(UINT32 func_idx) {
    Cur_env().Set_func_idx(func_idx);
    Cur_env().Set_cur_label_tab(_module.Get_label_tab(func_idx));
    Cur_env().Set_pc(_module.Get_expanded_func_base(func_idx));
  }
  // Exec environment status, _pc, _func_id, _fp[4]
  EXEC_CONTEXT &Cur_env()                          { return _current; }
  const EXEC_CONTEXT &Cur_env() const              { return _current; }

  // Function prologue and epilogue purpose
  void       Set_pc(AM_EXPAND *pc)            { Cur_env().Set_pc(pc);                       }

  // Execute some number of instructions.
  void Control_start(UINT64 instr_count);
  // Execute until finish
  void Exec_start();

  void            Func_prologue(UINT32 callee_idx);
  void            Func_epilogue();
  void Allocate_stack(UINT32 func_idx) {
    // allocate stack
    UINT32 stack_size = _module.Get_code(_module.Get_func(func_idx).Get_code_idx()).Get_stack_size();
    if (stack_size > 0) {
      char *csp = Cur_env().Get_sp();
      csp -= stack_size;
      Cur_env().Set_sp(csp);
    }
  }
  // return the real address to a global variable in global table
  //
  UINT64 Get_global_addr(UINT32 ofst) {
    return (UINT64) (_global_start + ofst);
  }
  INT32 Get_ofst_addr(UINT32 ofst, bool is_stack) {
#ifdef DIRECT_ACC_MEM
    if(is_stack) {
      return (INTPTR) (Cur_env().Get_sp() + ofst);
    } else {
      return (INTPTR) (_module.Get_glbl(ofst).Get_data_idx() + _mem_p);
    }
#else
    if(is_stack) {
      return Cur_env().Get_sp() + ofst - _sta_p;
    } else {
      return _module.Get_glbl(ofst).Get_data_idx();
    }
#endif
  }
  char* Get_base_addr(UINT32 reg_idx) {
#ifdef DIRECT_ACC_MEM
    char *ptr = (char *)REGADDR[reg_idx];
    if (ptr == 0) return NULL;
    Is_True(ptr > _mem_p && ptr <= _sta_p,
            ("Invalid memory access ptr=0x%x outof rang[0x%x, 0x%x]", ptr, _mem_p, _sta_p));
    return ptr;
#else
    INT32 ptr_ofs = REGADDR[reg_idx];
    // ptr_ofs != 0 is for the null-pointer check.
    // TODO: Add bounds check
#ifdef UWASM_MEM_CHECK
    Is_True_Rel(unsigned(abs(ptr_ofs)) < _mem_size,
               ("Invalid memory access ptr_ofs=%d, mem_size=%d", ptr_ofs, _mem_size));
    return _mem_p + ptr_ofs;
#else
    Is_True(unsigned(abs(ptr_ofs)) < _mem_size,
            ("Invalid memory access ptr_ofs=%d, mem_size=%d", ptr_ofs, _mem_size));
    return ptr_ofs > 0 ? _mem_p + ptr_ofs
                       : _sta_p + ptr_ofs;
#endif
#endif
  }
  bool Print_result_special(AM_EXPAND *inst, FILE *file) const;
// Execute functions of each opcode_l
#define EXEC_FUNC(func_name) void func_name (AM_EXPAND *inst);
#define DUP_FUNC(func_name)
#define UNUSED()
#include "u_func_list.def"
#undef EXEC_FUNC
#undef DUP_FUNC
#undef UNUSED

public:
  UWASM_MACHINE()                                                                     {}
  ~UWASM_MACHINE()                                                                    { if (_hea_p != NULL) UVM_heap_finalize(_hea_p); }
  UINT32            Init_memory(char *mem_p, UINT32 mem_size);
  INT32             Get_ret_val()                                                     { return _reg_i32[0]; }
  U_MODULE         &Get_u_module()                                                    { return _module; }
  void              Init_env(UINT32 func_idx);              // Goto specific function, as to setup the corrent pointers.
  STRVEC           &Get_string_args()              { return _str_args;  }
  const REGISTER<INT32> &Reg_i32()           const { return _reg_i32; }
  const REGISTER<INT64> &Reg_i64()           const { return _reg_i64; }
  const REGISTER<FLOAT32> &Reg_f32()         const { return _reg_f32; }
  const REGISTER<FLOAT64> &Reg_f64()         const { return _reg_f64; }
  REGISTER<INT32>       &Reg_i32()                 { return _reg_i32; }
  REGISTER<INT64>       &Reg_i64()                 { return _reg_i64; }
  REGISTER<FLOAT32>     &Reg_f32()                 { return _reg_f32; }
  REGISTER<FLOAT64>     &Reg_f64()                 { return _reg_f64; }
  char *Mem_pointer()                        const { return _mem_p; }
  char *Stack_pointer()                      const { return _sta_p; }
  char *Heap_pointer()                       const { return _hea_p; }
  UINT32 Heap_offset()                       const { return _hea_off; }
  UINT32 Mem_size()                          const { return _mem_size; }
  UINT32            Grow_memory(UINT32 target_size_bytes);
  void              Print_reg(VALTYPE valtype, UINT32 idx, FILE *file) const;
  void              Print_reg_ofst_by_print_type(PRINT_TYPE pt, UINT32 idx, FILE *file) const;
  void              Print_result(AM_EXPAND *inst, FILE *file, const char *slot1, const char *slot2) const;
  void              Print_result(AM_EXPAND *inst, FILE *file) const;
  void              Print_debug_info(AM_EXPAND *inst, FILE *file) const;
  void              Print(FILE * f);
  void              Print()                                                           { Print(stdout); }
  CONTEXT_STACK    &Get_stacktrace()               { return _ctx_stack; }
};

#endif //UWASM_VM_EXEC_UNIT_H
