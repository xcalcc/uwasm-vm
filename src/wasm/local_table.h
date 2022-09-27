/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

//
// Created by xc5 on 2020/8/12.
//

#ifndef UWASM_VM_LOCAL_TABLE_H
#define UWASM_VM_LOCAL_TABLE_H

#include "utilities/u_utils.h"
#include "trace/u_trace.h"
#include "u_am.h"
#include <cstdio>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

using std::string;
using std::vector;

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef TARG_UWASM
enum LABEL_IDX {
  LABEL_IDX_MIN = 0,
  LABEL_INVALID = 0,
  LABEL_NOT_NEEDED = 0x7FFFFFFF,
  LABEL_IDX_MAX = INT_MAX,
};
#else
#define LABEL_NOT_NEEDED 0x7FFFFFFF
#endif

enum BLOCK_IDX{
  BLOCK_FUNCTION = 1,
};

// Representing a local/temp entry for debugging.
enum LOCAL_IDX {
  LOCAL_IDX_MIN = 0,
  LOCAL_IDX_MAX = INT_MAX,
};


// Kind of the _elem in local table.
typedef enum {
  LOCAL,
  GLOBAL,
  TEMP,
  FORMAL_PARAM,
  CALL_PARAM,
  UNREACHABLE_VALUE,
  LOCAL_DUMMY,
} LOCAL_KIND;


// Constants
//
#define BLOCK_INSTR_BEGIN 1
#include "../runtime/u_runtime_reg.h"

class LABEL_ELEM {
private:
  UINT32 _pc;
public:
  UINT32 Get_pc() const { return _pc; }
  void Set_pc(UINT32 pc) { _pc = pc; }
  LABEL_ELEM()          : _pc(0)  {}
  LABEL_ELEM(UINT32 pc) : _pc(pc) {}
  void Print(FILE *file = stdout) {
    fprintf(file, " pc = 0x%08x\n", _pc);
  }
};

namespace uvm {
  class LABEL_TAB {
  private:
    vector<LABEL_ELEM>  _labels;
    vector<std::string> _label_name;
  public:
    LABEL_IDX Add(UINT32 pc, const char *name) {
      Is_True(pc != LABEL_IDX::LABEL_NOT_NEEDED, ("Should not be not-needed here."));
      Is_True(name != NULL, ("label name cannot be null."));
      LABEL_IDX cur = (LABEL_IDX) _labels.size();
      _labels.push_back(LABEL_ELEM(pc));
      _label_name.push_back(std::string(name));
      return cur;
    }

    LABEL_IDX Add(UINT32 pc, UINT32 block_id) {
      Is_True(pc != LABEL_IDX::LABEL_NOT_NEEDED, ("Should not be not-needed here."));
      LABEL_IDX cur = (LABEL_IDX) _labels.size();
      _labels.push_back(LABEL_ELEM(pc));
      std::string name;
      std::stringstream ss;
      ss << "block" << block_id;
      ss >> name; // blockX
      _label_name.push_back(name);
      return cur;
    }

    LABEL_IDX Add(UINT32 pc) {
      Is_True(pc != LABEL_IDX::LABEL_NOT_NEEDED, ("Should not be not-needed here."));
      LABEL_IDX cur = (LABEL_IDX) _labels.size();
      _labels.push_back(LABEL_ELEM(pc));
      _label_name.push_back(std::string(".anonymous"));
      return cur;
    }

    LABEL_ELEM &Get(LABEL_IDX idx) {
      Is_True(_labels.size() > idx, ("labels table does not contain idx = %d, out of range.", idx));
      return _labels[idx];
    }

    void Set(LABEL_IDX idx, const UINT32 pc) {
      Is_True(_labels.size() > idx, ("labels table does not contain idx = %d, out of range.", idx));
      Is_True(pc != LABEL_IDX::LABEL_NOT_NEEDED, ("Should not be not-needed here."));
      _labels[idx].Set_pc(pc);
    }

    LABEL_ELEM *Get(const char *name) {
      for (UINT32 idx = 0; idx < _label_name.size(); idx++) {
        if (strcmp(name, _label_name.at(idx).c_str()) == 0)
          return &_labels[idx];
      }
      Is_True(false, ("label name not found"));
      return NULL;
    }

    LABEL_IDX Get_label_idx(const char *name) {
      for (UINT32 idx = 0; idx < _label_name.size(); idx++) {
        if (strcmp(name, _label_name.at(idx).c_str()) == 0)
          return LABEL_IDX(idx);
      }
      return LABEL_NOT_NEEDED;
    }

    UINT32 Size() { return (UINT32)_labels.size(); }

    void Print(FILE *file = stdout) {
      fprintf(file, ">>>>>>>> Printing the label table >>>>>>>>\n");
      fprintf(file, " Label count = %u \n", (uint32_t)_labels.size());
      for (UINT32 i = 0; i < _labels.size(); i++) {
        LABEL_ELEM &elem = _labels[i];
        fprintf(file, " // Label ID : 0x%08x [%.18s] \t ", i, _label_name[i].c_str());
        elem.Print(file);
      }
      fprintf(file, "<<<<<<<< End of the label table <<<<<<<<\n");
    }
  };
}


class LOCAL_TAB_ELEM {
private:
  LOCAL_KIND  _kind;
  VALTYPE     _val_type;
  LOCAL_IDX   _home_local_idx;  // local idx in wasm, only for locals
  // debug info
  UINT64       _push_stmt;      // which line(wasm inst offset) did push
public:
  LOCAL_TAB_ELEM(LOCAL_KIND kind, VALTYPE val_type, UINT64 push_stmt=0) :
    _kind(kind), _val_type(val_type), _push_stmt(push_stmt) {}
  LOCAL_KIND Kind()                 const        { return _kind;                }
  VALTYPE    ValType()              const        { return _val_type;            }
  LOCAL_IDX  Home_local_idx()       const        { return _home_local_idx;      }
  UINT64     Push_stmt()            const        { return _push_stmt;           }
  void       Push_stmt(UINT64 push_stmt)         { _push_stmt = push_stmt;      }
  void       Set_local_idx(LOCAL_IDX local_idx)  { _home_local_idx = local_idx; }
};


class LOCAL_MAP_ELEM {
private:
  UINT32         _reg_idx;
  VALTYPE        _ty;
  LOCAL_KIND     _map_kind;
public:
  VALTYPE        Get_ty()               const { return _ty;       }
  UINT32         Get_reg_idx()          const { return _reg_idx;  }
  LOCAL_KIND     Get_map_kind()         const { return _map_kind; }
  LOCAL_MAP_ELEM(UINT32 reg_idx, VALTYPE ty, LOCAL_KIND kind) :
    _reg_idx((UINT32) (reg_idx + RET_REG_NUM + CONST_REG_NUM - 1)),
    _ty(ty),
    _map_kind(kind) {
    Is_True(reg_idx >= 1, ("Incorrect register idx given = %d, "
                           "should be at least 1.", reg_idx));
  }
};


class LOCAL_TAB {
private:
  vector<LOCAL_TAB_ELEM> _content;
  UINT32 _max_call_param;
public:
  LOCAL_TAB() : _content(), _max_call_param(0) {}
  const vector<LOCAL_TAB_ELEM> *Get_content() { return &_content; }
  void push_back(LOCAL_TAB_ELEM &elem) { _content.push_back(elem); }

  UINT32 Get_max_call_param() const { return _max_call_param; }
  void Set_max_call_param(UINT32 max_call_param) { _max_call_param = max_call_param; }
  UINT32 size() const { return (UINT32)_content.size(); }
  bool empty()        { return _content.empty(); }
  LOCAL_TAB_ELEM &Get(UINT32 idx) {
    Is_True(_content.size() > idx, ("Invalid local table index = %d", idx));
    return _content.at(idx);
  }
};
typedef vector<LOCAL_MAP_ELEM>      LOCAL_MAP;


#endif //UWASM_VM_LOCAL_TABLE_H
