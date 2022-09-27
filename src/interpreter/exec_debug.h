//
// Created by xc5 on 4/2/21.
//

#ifndef UWASM_VM_EXEC_DEBUG_H
#define UWASM_VM_EXEC_DEBUG_H

#include "exec_unit.h"
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

enum BP_STATUS{
  BP_ENABLED       = 1,
  BP_ENABLED_NEXT  = 2,
  BP_DISABLED      = 100,
};

class UWASM_BP{
  UINT32 _bp_id;
  UINT32 _module_num;
  UINT32 _func_id;
  UINT32 _pc;
  UINT32 _stack_tos;
  AM_EXPAND *_inst_pc;
  BP_STATUS  _status;
public:
  UWASM_BP(UINT32 bp_id, UINT32 mod_id, UINT32 func_id, UINT32 ubh_pc, AM_EXPAND *inst_pc, BP_STATUS status, UINT32 stack_tos) :
    _bp_id(bp_id),
    _module_num(mod_id), _func_id(func_id),
    _pc(ubh_pc), _inst_pc(inst_pc), _status(status), _stack_tos(stack_tos) {}
  UINT32       Get_func_id()     const { return _func_id;      }
  UINT32       Get_ubh_pc()      const { return _pc;           }
  AM_EXPAND   *Get_inst_pc()     const { return _inst_pc;      }
  UINT32       Get_bp_id()       const { return _bp_id;        }
};

class UWASM_WP{
  UINT32     _wp_id;
  UINT32     _module_num;
  UINT32     _ofst;
  char      *_addr;
  BP_STATUS  _status;
public:
  UWASM_WP(UINT32 wp_id, UINT32 mod_id, UINT32 ofst, char *inst_pc, BP_STATUS status) :
    _wp_id(wp_id), _module_num(mod_id),
    _ofst(ofst), _addr(inst_pc), _status(status) {}
  UINT32       Get_ofst()        const { return _ofst;           }
  char        *Get_addr()        const { return _addr;           }
  UINT32       Get_wp_id()       const { return _wp_id;          }
};


class PC_EXP_MAPPING {
  UINT32 _func_id;
  UINT32 _ubh_pc;
  AM_EXPAND * _exp;
};

union UBH_PC_INFO {
  struct {
    UINT32 _func_id;
    UINT32 _ubh_pc;
  };
  UINT64 _hashval;
};

class UWASM_DEBUG {
  friend class UWASM_MACHINE;
  friend class UWASM_SYSTEM;
  friend class UWASM_PERF;
private:
  vector<UWASM_BP> _breakpoints;
  set<AM_EXPAND *> _breakpoints_cache;
  vector<UWASM_WP> _watchpoints;
  set<UINT32>      _watchpoints_cache;
  UWASM_MACHINE   *_machine;

  // Debugging / Breakpoint info
  AM_EXPAND       *_cur_pc = nullptr;
  EXEC_CONTEXT    *_current_dbg_frame = nullptr;
  UINT32           _current_dbg_id = 0;
  UBH_PC_INFO      _current_dbg_pc;

  UINT32           _bp_count = 0;
  UINT32           _wp_count = 0;
  FILE            *_output_file = stderr;
public:
  UWASM_DEBUG(): _machine(nullptr), _bp_count(0), _breakpoints(), _breakpoints_cache() {}
  void             Init(UWASM_MACHINE *machine) { _machine = machine;   }
  UINT32           Get_bp_count()        const  { return _bp_count;     }
  const vector<UWASM_BP> &Get_breakpoints()       const { return _breakpoints;}
  const set<AM_EXPAND *> &Get_breakpoints_cache() const { return _breakpoints_cache;}
  const vector<UWASM_WP> &Get_watchpoints()       const { return _watchpoints;}
  const set<UINT32>      &Get_watchpoints_cache() const { return _watchpoints_cache;}
  UWASM_MACHINE          *Get_machine()           const { return _machine;}
  void Add_breakpoint(UINT32 func_id, UINT32 ubh_pc, AM_EXPAND *inst_pc, UINT32 stack_tos) {
    Is_True(_machine != NULL, ("to add breakpoint, you need to init first."));
    _breakpoints.emplace_back(_bp_count, 0, func_id, ubh_pc, inst_pc, BP_STATUS::BP_ENABLED, stack_tos);
    _breakpoints_cache.insert(inst_pc);
    _bp_count += 1;
  }
  void Add_watchpoint(UINT32 ofst, char *memofst) {
    Is_True(_machine != NULL, ("to add breakpoint, you need to init first."));
    _watchpoints.emplace_back(_wp_count, 0, ofst, memofst, BP_STATUS::BP_ENABLED);
    _watchpoints_cache.insert(ofst);
    _wp_count += 1;
  }
  BOOL Is_ofst_watched(UINT32 ofst);
  BOOL Is_inst_breakable(AM_EXPAND *exp);
  BOOL Is_valid_breakpoint(AM_EXPAND *exp);
  EXEC_CONTEXT *Get_current_frame();
  EXEC_CONTEXT *Change_current_frame(UINT32 id);
  void Print_one_frame(UINT32 id, EXEC_CONTEXT *ctx);
  void Print_current_frame();
  void Print_current_func();
  void Print_callstack();
  void On_breakpoint(AM_EXPAND *exp);
  FILE *Get_stderr();
  void Load_file_and_module(U_MODULE &module);
  void Print();
  void Print_func(UINT32 func_idx);
};
UWASM_DEBUG & Uvm_debug_tracker();
#endif //UWASM_VM_EXEC_DEBUG_H
