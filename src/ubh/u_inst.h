/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_INSTR_INFO_H
#define UWASM_VM_U_INSTR_INFO_H

#include <vector>
#include <string.h>
#include <stdio.h>
#include "u_types.h"
#include "u_am.h"
#include "trace/u_trace.h"
#include "ubh/u_opcode.h"

using namespace std;

#define _7BIT_REG_MAX 128
#define _10BIT_REG_MAX 1024
#define UNINIT_LABEL_IDX -1
#define U_INST32_SIZE 4
#define U_INST64_SIZE 8

/*
 * instruction register print type
 * part of instructions contains multi registers, and those registers have different type
 * not same with instruction value type
 * those type information don't have side effect on runtime
 */
enum PRINT_TYPE {
  P_NONE,
  P_I32,
  P_U32,
  P_I64,
  P_U64,
  P_F32,
  P_F64,
  P_OFS,
};

struct U_SPECIAL_FORMAT {
  U_OPCODE_LONG     opcode_l;
  PRINT_TYPE        pt_arr[3];
};

extern U_SPECIAL_FORMAT SPECIAL_FORMAT_ARR[];
extern UINT32 SPECIAL_FORMAT_ARR_LEN;

/*
 * uvm instruction encode format have 2 kind of length, one is 32-bit, another is 64-bit
 * part of instructions just have 32-bit mode, like call, jump indirect and so on
 * lots of instructions have 2 kind of modes, like add, sub, or
 * for cg, should use 32-bit instruction as much as possible, thus can reduce the code size
 *
 * we push 32-bit and 64-bit instructions into one vector, so the instruction is variable length
 * should use U_INST_ITER to iterate all instructions
 * and we MUST make sure the encoder and decoder are little-endian, so the first 8 bit is opcode_l
 * can help us to know about the TY_AM, then we can know how many bits we should to read
 * if encoder and decoder is big-endian, we can't archive this
 */
union U_INST32 {
public:
  // TY_AM32
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _dummy     : 22;
  };
  // TY_AM32_OFS
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _ofs       : 22;
  } OFS;
  // TY_AM32_R
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _dummy     : 12;
    UINT32 _r1        : 10;
  } R;
  // TY_AM32_R_IMM
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 10;
    UINT32 _dummy     : 2;
    INT32  _imm       : 10;
  } R_IMM;
  // TY_AM32_R_OFS
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 10;
    UINT32 _dummy     : 2;
    INT32  _ofs       : 10;
  } R_OFS;
  // TY_AM32_R_R
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 10;
    UINT32 _dummy     : 2;
    UINT32 _r2        : 10;
  } R_R;
  // TY_AM32S_R_R_IMM
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 7;
    UINT32 _r2        : 7;
    INT32  _imm       : 8;
  } R_R_IMM;
  // TY_AM32S_R_R_OFS
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 7;
    UINT32 _r2        : 7;
    UINT32 _ofs       : 8;
  } R_R_OFS;
  // TY_AM32S_R_R_R
  struct {
    UINT32 _opcode    : 8;
    UINT32 _ty        : 2;
    UINT32 _r1        : 7;
    UINT32 _r2        : 7;
    UINT32 _dummy     : 1;
    UINT32 _r3        : 7;
  } R_R_R;

  U_INST32(UINT32 inst)                               { memcpy(this, (char *) &inst, 4); }
  U_INST32(UINT64 inst)                               { UINT32 inst32 = (UINT32) inst; memcpy(this, (char *) &inst32, 4); }
  UINT32 Opcode()                               const { return _opcode; }
  VALTYPE Valtype()                             const { return (VALTYPE) _ty; }
  TY_AM Ty_am()                                 const { return UWINST_TY_AM[Opcode()]; }
  UINT32 Inst_value()                                 { return *((UINT32 *) this); }
  void Print(FILE *file) const;
};

union U_INST64 {
public:
  // dummy struct
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT64   _dummy   : 54;
  };
  // TY_AM64_R_IMM
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT32   _r1      : 10;
    UINT32   _dummy   : 12;
    INT32    _imm     : 32;
  } R_IMM;
  // TY_AM64_R_OFS
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT32   _r1      : 10;
    UINT32   _dummy   : 12;
    UINT32   _ofs     : 32;
  } R_OFS;
  // TY_AM64_R_R_IMM
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT32   _r1      : 10;
    UINT32   _dummy   : 12;
    INT32    _imm     : 22;
    UINT32   _r2      : 10;
  } R_R_IMM;
  // TY_AM64_R_R_OFS
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT32   _r1      : 10;
    UINT32   _dummy   : 12;
    UINT32   _ofs     : 22;
    UINT32   _r2      : 10;
  } R_R_OFS;
  // TY_AM64_R_R_R
  struct {
    UINT32   _opcode  : 8;
    UINT32   _ty      : 2;
    UINT64   _dummy   : 24;     // this field cross the word, need to be align with 64 bit
    UINT32   _r1      : 10;
    UINT32   _r2      : 10;
    UINT32   _r3      : 10;
  } R_R_R;

  U_INST64(UINT64 inst)                               { memcpy(this, (char *) &inst, 8); }
  UINT32 Opcode()                               const { return _opcode; }
  VALTYPE Valtype()                             const { return (VALTYPE) _ty; }
  TY_AM Ty_am()                                 const { return UWINST_TY_AM[Opcode()]; }
  UINT64 Inst_value()                                 { return *((UINT64 *) this); }
  void Print(FILE *file) const;
};

class U_INST_UTIL {
private:
  static void Check_content_size(UINT32 required, UINT32 actual) {
    Is_True_Rel(required == actual, ("Content size is wrong, required: %d, actual: %d.", required, actual));
  }
  static void Check_imm(UINT32 bit, UINT32 imm) {
    UINT64 u_upper_bound = ((UINT64) 1u) << bit;
    INT32 i_upper_bound = 1 << (bit - 1);
    INT32 i_lower_bound = -1 * i_upper_bound;
    bool in_u_bound = (imm < u_upper_bound) ? true : false;
    bool in_i_bound = i_lower_bound <= (INT32) imm && (INT32) imm < i_upper_bound;
    Is_True_Rel(in_u_bound || in_i_bound, ("Imm out of range, bit len: %d, imm: %d.", bit, imm));
  }
  static void Check_reg(UINT32 max_idx, UINT32 reg_idx) {
    Is_True_Rel(reg_idx <= max_idx, ("Register index out of range, idx: %d.", reg_idx));
  }
public:
  static void Init_inst32(UINT32 *inst, TY_AM ty_am, UINT32 opcode, VALTYPE valtype, vector<UINT32> &content);
  static void Init_inst64(UINT64 *inst, TY_AM ty_am, UINT32 opcode, VALTYPE valtype, vector<UINT32> &content);
  static bool Is_32bit(TY_AM ty_am);
  static bool Is_32bit(UINT32 opcode)                 { return ((UINT8) opcode & 0x80u) == 0; }
  static void Print_reg_with_pt(PRINT_TYPE pt, UINT32 idx, FILE *file);
  static void Print_reg_with_valty(VALTYPE valtype, UINT32 idx, FILE *file);
  static void Print_reg(INT32 s_idx, INT32 t_idx, VALTYPE valtype, UINT32 idx, FILE *file);
  static INT32 Get_special_format(U_OPCODE_LONG opcode_l);
  static UINT32 Opcode_to_opcode_l(UINT32 opcode, VALTYPE valtype)     { return (opcode << VALTY_SZ) + valtype; }
};

class U_INST_ITER {
private:
  UINT32 *_p;

public:
  U_INST_ITER(UINT32 *p) : _p(p) {}
  pair<bool, UINT64> operator*() {
    U_INST32 *inst = (U_INST32 *) _p;
    bool is_inst32 = U_INST_UTIL::Is_32bit(inst->_opcode);
    if (is_inst32) {
      // deference 32-bit, and cast to 64-bit, and then return
      return make_pair(true, (UINT64) (*_p));
    } else {
      return make_pair(false, *((UINT64 *) _p));
    }
  }
  U_INST_ITER& operator++() {
    U_INST32 *inst = (U_INST32 *) _p;
    bool is_inst32 = U_INST_UTIL::Is_32bit(inst->_opcode);
    if (is_inst32) {
      _p += 1;
    } else {
      _p += 2;
    }
    return *this;
  }
  U_INST_ITER operator++(int) {
    UINT32 *tmp = _p;
    U_INST32 *inst = (U_INST32 *) _p;
    bool is_inst32 = U_INST_UTIL::Is_32bit(inst->_opcode);
    if (is_inst32) {
      _p += 1;
    } else {
      _p += 2;
    }
    return U_INST_ITER(tmp);
  }
  bool operator==(U_INST_ITER &iter)                 { return iter._p == _p; }
  bool operator!=(U_INST_ITER &iter)                 { return iter._p != _p; }
  UINT32 *Addr()                               const { return _p; }
};

class U_OPC_TY_PAIR {
private:
  // address mode, 64 bit length
  // one inst union class may have 2*32 bit inst or 1*64 bit inst
  UINT32   _opcode;
  VALTYPE  _valty;
public:
  U_OPC_TY_PAIR(UINT16 uwasm_op, VALTYPE ty) {
    _opcode = uwasm_op;
    _valty = ty;
  }
  UINT32 Get_opcode() {
    return _opcode;
  }
  VALTYPE Get_valty() {
    return _valty;
  }
  TY_AM Get_ty_am() {
    return UWINST_TY_AM[Get_opcode()];
  }
  void Set_valty(VALTYPE valty) {
    _valty = valty;
  }
  void Set_opcode(UINT32 opcode) {
    _opcode = opcode;
  }
};

#endif //UWASM_VM_U_INSTR_INFO_H
