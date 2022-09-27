/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef WASM2UWASM_CONVERTER_H
#define WASM2UWASM_CONVERTER_H

#include "ubh/u_opcode.h"
#include "trace/u_trace.h"
#include "encoder/u_encoder.h"
#include "u_types.h"
#include "local_table.h"
#include "winst.h"
#include "reader.h"
#include "ubh/ubh.h"
#include <vector>
#include <stack>
#include <list>
#include <unordered_map>
#include "conv_trace.h"
#include "conv_map.h"
using namespace std;

// wasm stack track
//
class WASM_STACK_ELEM {
private:
  LOCAL_KIND  _kind;        // type: local, global, temp
  VALTYPE     _val_type;
  UINT32      _idx;         // reg id, maximum 4092
  UINT32      _push_stmt;   // stmt_id or ins_cnt.
public:
  WASM_STACK_ELEM(LOCAL_KIND kind, VALTYPE val_type, UINT32 idx, UINT32 ins_cnt) :
                  _kind(kind), _val_type(val_type), _idx(idx), _push_stmt(ins_cnt){};
  LOCAL_KIND Get_kind()     const   { return _kind;  }
  UINT32     Get_idx()      const   { return _idx;   }
  VALTYPE    Get_val_type() const   { return _val_type;  }
  UINT32     Get_push_stmt() const  { return _push_stmt; }
  void       Print(FILE *file = stderr) {
    fprintf(file, "%-4d  %-4d  %-5d  %-7d \n",
            Get_kind(),
            Get_val_type(),
            Get_idx(),
            Get_push_stmt());
  }
};

typedef vector<WASM_INST>::iterator WINSITER;
typedef std::list<UINT32>           UINTLIST;
typedef std::vector<UINT32>         UINTVEC;
typedef stack<WASM_STACK_ELEM>      WASMSTK;

enum BLK_KIND {
  BLK_NONE    = 0, // used for dummy placement at the beginning of the vec
  BLK_IF      = 1,
  BLK_BLOCK   = 2,
  BLK_ELSE    = 3,
  BLK_LOOP    = 4,
  BLK_FUNC    = 5,
};


enum WINST_STMT_KIND {
  DUMMY,
  WINST_STMT_BLOCK,
  WINST_STMT_ROOT_BLOCK,
  WINST_STMT_INSTR
};

// wasm code stmt (entry of block)
class WINST_STMT {
  WINST_STMT_KIND  _stmt_kind;
  UINT32           _id; // stmt id.
  UINT32           _opcode;
// Parent and children
  UINT32    _parent_stmt;
  UINTVEC   _children;
// Begin & End pc ?
  UINT32    _stmt_begin_pc = 0;
  UINT32    _stmt_last_pc = 0;
  UINT32    _block_id      = 0; // parent block?
  UINT32    _begin_label   = 0;
  UINT32    _end_label     = 0;
  UINT32    _else_stmt_id  = 0;
  UINT32    _result_cnt  = 0;
  UINT32    _result_type = 0;
  UINT32    _result_reg  = 0;
  UINT32    _stack_size  = 0;
public:
  WINST_STMT(WINST_STMT_KIND stmtKind, UINT32 stmtBeginPc, UINT32 opc) :
  _stmt_kind(stmtKind), _stmt_begin_pc(stmtBeginPc), _block_id(0),
  _opcode(opc), _stack_size(0) {
    Is_True(stmtKind  != WINST_STMT_KIND::WINST_STMT_BLOCK, ("Block must has a block id."));
    Init();
  }

  WINST_STMT(WINST_STMT_KIND stmtKind, UINT32 stmtBeginPc,
             UINT32 block_id,  UINT32 opc) :
    _stmt_kind(stmtKind), _stmt_begin_pc(stmtBeginPc),
    _block_id(block_id), _opcode(opc), _stack_size(0) {
    Init();
  }

  void Init();

  WINST_STMT_KIND Get_stmt_kind() const {
    return _stmt_kind;
  }

  void Set_stmt_kind(WINST_STMT_KIND stmtKind) {
    _stmt_kind = stmtKind;
  }

  UINT32 Get_stmt_begin_pc() const {
    return _stmt_begin_pc;
  }

  void Set_stmt_begin_pc(UINT32 stmtBegin_pc) {
    _stmt_begin_pc = stmtBegin_pc;
  }

  void Set_blk_id(UINT32 i) {
    _block_id = i;
  }
  UINT32 Get_begin_label()               { return _begin_label;     }
  UINT32 Get_end_label()                 { return _end_label;       }
  void Set_begin_label(UINT32 i)         { _begin_label = i;        }
  void Set_end_label(UINT32 i)           { _end_label = i;          }
  UINT32      Get_result_type()          { return _result_type;     }
  UINT32      Get_result_cnt()           { return _result_cnt;      }
  UINT32      Get_result_reg()           { return _result_reg;      }
  void        Set_result_type(UINT32 i)  { _result_type = i;        }
  void        Set_result_cnt(UINT32 i)   { _result_cnt = i;         }
  void        Set_result_reg(UINT32 i)   { _result_reg = i;         }
  void Set_parent(UINT32 i)              { _parent_stmt = i;        }
  UINT32 Get_parent()                    { return _parent_stmt;     }
  void Add_children(UINT32 i)            { _children.push_back(i);  }
  UINTVEC &Get_children()                { return _children;        }
  void Set_id(UINT32 i)                  { _id = i;                 }
  UINT32 Get_id()                        { return _id;              }
  UINT32 Get_opcode()                    { return _opcode;          }
  void Set_else_stmt(UINT32 i)           { _else_stmt_id = i;       }
  UINT32 Get_else_stmt()                 { return _else_stmt_id;    }
  BOOL  Has_else()                       { return _else_stmt_id != 0; }
  void Set_stack_size(UINT32 i)          { _stack_size = i;         }
  UINT32 Get_stack_size()                { return _stack_size;      }
};

typedef vector<WINST_STMT> W_STMTS;
class WINST_STMT_GRAPH {
private:
  W_STMTS _stmts;
  map<UINT32, UINT32> _ins_to_stmt;
public:
  WINST_STMT_GRAPH() : _stmts(), _ins_to_stmt() {
    _stmts.clear(); _ins_to_stmt.clear();
  }
  W_STMTS &Get_stmts() { return _stmts; }

  WINST_STMT &Stmt_id(UINT32 id) {
    Is_True(id < _stmts.size(), ("Incorrect stmt id = %d, size = %lu", id, _stmts.size()));
    return _stmts.at(id);
  }
  WINST_STMT &Stmt_by_inst(UINT32 ins_cnt) {
    Is_True_Rel(_ins_to_stmt.find(ins_cnt) != _ins_to_stmt.end(),
                ("Cannot find such instr in stmt graph %d", ins_cnt));
    return Stmt_id(_ins_to_stmt.find(ins_cnt)->second);
  }
  UINT32 Get_stmt_id_by_inst(UINT32 ins_cnt) {
    Is_True_Rel(_ins_to_stmt.find(ins_cnt) != _ins_to_stmt.end(),
                ("Cannot find such instr in stmt graph %d", ins_cnt));
    return _ins_to_stmt.find(ins_cnt)->second;
  }
  // Add a stmt to the graph
  UINT32 Add_stmt(const WINST_STMT &item, UINT32 ins_cnt) {
    UINT32 id = _stmts.size();
    _stmts.push_back(item);
    _stmts.at(id).Set_id(id);
    if (item.Get_stmt_kind() != WINST_STMT_KIND::WINST_STMT_ROOT_BLOCK) {
      _ins_to_stmt.insert(std::make_pair(ins_cnt, id));
    }
    return id;
  }

  UINT32 Add_stmt_to_parent(const WINST_STMT &stmt, UINT32 ins_cnt, UINT32 parent) {
    UINT32 current = Add_stmt(stmt, ins_cnt);
    Stmt_id(current).Set_parent(parent);
    Stmt_id(parent).Add_children(current);
    return current;
  }
};

void Wasm_to_uwasm(U_ENCODER &encoder, WINSVEC &instr, CONST_BUFFER inst_buf, UINT32 len, FILE_CONTEXT &ctx);

class CONVERT_OPTIONS {
  std::string _output_name;
  bool       _conv_reading;
public:
  void Set_conv_in_read(bool v) {
    _conv_reading = v;
  }

  bool Is_conv_in_read() {
    return _conv_reading;
  }

  void Set_output_name(const char *n) {
    _output_name = n;
  }

  const char *Get_output_name() {
    return _output_name.c_str();
  }

  CONVERT_OPTIONS() {
    _output_name  = "out.uwasm";
    _conv_reading = true;
  }
};


enum HIER_STATE {
    STATE_BEGIN = 0,
    STATE_FINISHED = 1,
    STATE_INVALID = 3,
    STATE_NOT_NEEDED = 4,
    STATE_LABEL_ID = 5,
    STATE_BLOCK = 6,
};

class HIER_SEARCH_ITER {
private:
    UINT32 _block_id;
    HIER_STATE _state;
public:
    HIER_SEARCH_ITER(LABEL_IDX lbl) : _block_id(lbl), _state(STATE_BEGIN) {}
    HIER_SEARCH_ITER(UINT32 lbl) : _block_id(lbl), _state(STATE_BEGIN) {}
    HIER_SEARCH_ITER(LABEL_IDX lbl, HIER_STATE state) : _block_id(lbl), _state(state) {}
    HIER_SEARCH_ITER(UINT32 lbl, HIER_STATE state) : _block_id(lbl), _state(state) {}
    HIER_SEARCH_ITER(HIER_STATE state) : _block_id(0), _state(state) {}
    UINT32 Get_blk_id()   { return _block_id; }
    UINT32 Get_state()    { return _state;    }
    LABEL_IDX Get_label_id() {
      Is_True(_state == STATE_LABEL_ID, ("This is not a label here."));
      Is_True(_block_id != LABEL_IDX::LABEL_NOT_NEEDED, ("Label idx should not be LABEL_NOT_NEEDED here."));
      return (LABEL_IDX) _block_id;
    }
//    bool operator== (LABEL_IDX value) {
//      // compare the state
//      if ((_state == HIER_STATE::STATE_NOT_NEEDED && value == LABEL_NOT_NEEDED) ||
//          (_state == HIER_STATE::STATE_INVALID && value == LABEL_INVALID)) {
//        return true;
//      }
//      return _block_id == value;
//    }
    bool Is_invalid() { return _state == HIER_STATE::STATE_INVALID; }
    bool Is_not_needed() { return _state == HIER_STATE::STATE_NOT_NEEDED; }
};


enum REG_ALLOC_STATE{
    ALLOC_FREE = 0,
    ALLOC_TEMP_OCCUPIED = 1,
    ALLOC_WASM_LOCAL = 2,
    ALLOC_RETURN_REG = 3,
    ALLOC_OTHER = 3,
};

class REG_ALLOC_HELPER {
public:
    REG_ALLOC_HELPER(): _next_reg_id{0}, _register_state() {
      // reserve the default 4096 registers for each valty.
      for (int i = 0; i < VALTY_COUNT; i ++) {
        _register_state[i].resize(4096, REG_ALLOC_STATE::ALLOC_FREE);
      }
    }
    UINT32 Preoccupy_registers(VALTYPE valty, UINT32 basic, UINT32 local_count) {
      _next_reg_id[valty] = basic + local_count;
      for (UINT32 i = 0; i < basic; i++) {
        _register_state[valty][i] = REG_ALLOC_STATE::ALLOC_RETURN_REG;
      }
      for (UINT32 i = 0; i < local_count; i++) {
        _register_state[valty][basic + i] = REG_ALLOC_STATE::ALLOC_WASM_LOCAL;
      }
    }
    UINT32 Get_available_reg(VALTYPE valty) {
      Is_True_Rel(_next_reg_id[valty] < REG_L_MAX, ("Register index out of bound, not handled."));
      UINT32 reg_id = _next_reg_id[valty] ++;
      Is_True(_register_state[valty][reg_id] == REG_ALLOC_STATE::ALLOC_FREE,
              ("Register is not free reg = valty:%d, id:%d.", valty, reg_id));
      _register_state[valty][reg_id] = REG_ALLOC_STATE::ALLOC_TEMP_OCCUPIED;
      return reg_id;
    }
    void Free_register(VALTYPE valty, UINT32 reg_id) {
      if (reg_id >= 126) {
        return;
      }
      // set as free.
      Is_True(REG_ALLOC_STATE::ALLOC_TEMP_OCCUPIED == _register_state[valty][reg_id],
              ("(Valty = %d), Register %d is not at temp_occupied, thus not freeable. state = %d",
                valty, reg_id, _register_state[valty][reg_id]));
      _register_state[valty][reg_id] = REG_ALLOC_STATE::ALLOC_FREE;
      Is_True(reg_id == _next_reg_id[valty] - 1,
              ("Not Top-of-stack register freed ?, TOS = %d, freed = %d",
                _next_reg_id[valty] - 1, reg_id));
      _next_reg_id[valty] --;
    }
private:
    UINT32                      _next_reg_id[VALTY_COUNT];
    vector<REG_ALLOC_STATE>     _register_state[VALTY_COUNT];
};

CONVERT_OPTIONS &Get_conv_opt();

UINT32 Create_new_block(U_ENCODER &encoder, stack<UINT32> &work, WINST_STMT_GRAPH &graph,
                        UINT32 opc, UINT32 block_type,
                        UINT32 pc, BLK_KIND kind);

UINT32 Finish_old_block(stack<UINT32> &stack, UINT32 pc);

UINT32 Add_label(U_ENCODER &encoder, BOOL begin, UINT32 pc, BOOL add_new_label);

HIER_SEARCH_ITER Get_label_index_for_branch(UINT32 ins_cnt, WINST_STMT_GRAPH &graph, UINT32 jump_cnt);

UINT32 Get_parent_stmt_for_branch(UINT32 ins_cnt, WINST_STMT_GRAPH &graph, UINT32 jump_cnt);

void Add_mov_inst(UINT32 rd, UINT32 rs, VALTYPE valty, U_ENCODER &encoder, WINST_STMT &stmt);

TY_AM Get_opc_tyam(U_OPCODE opc);

#endif //WASM2UWASM_CONVERTER_H
