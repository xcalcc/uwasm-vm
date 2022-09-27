/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef WASM2UWASM_WINST_H
#define WASM2UWASM_WINST_H

#include <utility>
#include <vector>
#include <string>
#include "type_enum.h"

typedef enum {
  VALUES       = 0,
  TYPES        = 1,
  INSTRUCTIONS = 2,
  MODULES      = 3,
  ENDFILE      = 4
} CODE_CLASS;


typedef enum {
  INST_CLASS_MEM     = 0,
  INST_CLASS_NUM     = 1,
  INST_CLASS_VAR     = 2,
  INST_CLASS_PARM    = 3,
  INST_CLASS_CTRL    = 4,
  INST_CLASS_CONST   = 5,
} INST_CLASS;


typedef enum {
  NUM_CONST  = 0,           // numeric constant
  NUM_TEST   = 1,           // numeric test
  NUM_CMP    = 2,           // numeric comparison
  NUM_UNARY  = 3,           // numeric unary
  NUM_BINARY = 4,           // numeric binary
  NUM_CVTL   = 5,           // numeric conversion
} NUM_TY;


typedef enum {
  CTRL_NOOP     = 0,
  CTRL_BLOCK    = 1,
  CTRL_IF       = 2,
  CTRL_END      = 3,
  CTRL_ELSE     = 4,
  CTRL_LOOP     = 5,
  CTRL_BR       = 6,
  CTRL_BR_IF    = 7,
  CTRL_BR_TABLE = 8,
  CTRL_RETURN   = 9,
  CTRL_UNREACH  = 10,
} CTRL_TY;


class WINST_MEMORY;
class WINST_NUMERIC;
class WINST_VAR;


// variable size WASM instructions
//
class WASM_INST {
private:
  const char  *_name;       // name of the instruction
  UINT8        _encode;     // byte size code
  VALTYPE    _size1;
  VALTYPE    _size2;
  INST_CLASS   _class;      // instruction class
  UINT8        _numopr;     // number of operands
  UINT64       _pc;         // pc for translator to use.
public:
  UINT8        Encode(void)    const      { return _encode;  }
  VALTYPE    OpSz1(void)     const      { return _size1;   }
  VALTYPE    OpSz2(void)     const      { return _size2;   }
  INST_CLASS   InstClass(void) const      { return _class;   }
  UINT8        Numopr(void)    const      { return _numopr;  }
  UINT64       Pc(void)        const      { return _pc;      }
  void         Set_pc(UINT64 pc)          { _pc = pc;        }
  const char  *Get_name()      const      { return _name;    }
  WASM_INST(const char *name, UINT8 encode, VALTYPE sz1, VALTYPE sz2,
            INST_CLASS ins, UINT8 numopr) :
            _name(name), _encode(encode), _size1(sz1), _size2(sz2),
            _class(ins), _numopr(numopr), _pc(0) {}
  UINT8 Print_opr (FILE *file, VALTYPE opr_size, CONST_BUFFER pc);
  INT32 Print(FILE *file = stdout);
  INT32 Print(CONST_BUFFER pc, FILE *file = stdout);
  INT32 Print(CONST_BUFFER pc, UINT32 ins_cnt, FILE *file = stdout);
  INT32 Print(UINT32 ins_cnt, FILE *file);
  UINT8 Emit_opr (FILE *file, UINT8 opr_size, CONST_BUFFER pc);
  INT32 Emit_binary(CONST_BUFFER pc, FILE *file = stderr);
  UINT32 Read_inst_size(CONST_BUFFER pc);
  UINT64 Read_opr(UINT8 opr_size, CONST_BUFFER pc);
  UINT32 Read_opr_size_valty(const UINT8 *pc, VALTYPE valty) const;
}; // end of class WASM_INST


// section ids
//
typedef enum {
    CUSTOM_SECTION     = 0,
    TYPE_SECTION       = 1,
    IMPORT_SECTION     = 2,
    FUNCTION_SECTION   = 3,
    TABLE_SECTION      = 4,
    MEMORY_SECTION     = 5,
    GLOBAL_SECTION     = 6,
    EXPORT_SECTION     = 7,
    START_SECTION      = 8,
    ELEM_SECTION       = 9,
    CODE_SECTION       = 10,
    DATA_SECTION       = 11,
    DATA_COUNT_SECTION = 12,
    EVENT_SECTION      = 13,
} WASM_SECTION_ID;


class WASM_SECTION {
private:
    WASM_SECTION_ID  _id;
    UINT32      _size;  // size of contents in bytes
    UINT32      _num;   // num of ...(specify by different section)
public:
    WASM_SECTION(WASM_SECTION_ID id, UINT32 size, UINT32 num) : _id(id), _size(size), _num(num) {};

    WASM_SECTION_ID          Id(void)      const   { return _id;   }
    UINT32              Size(void)    const   { return _size; }
    UINT32              Num(void)     const   { return _num; }
};


// code section has a sequence of functions
// a function has:
//   declaration of locals
//   function body as an expression
// a local declaration has:
//   a u32 count
//   a value type
class FUNCTION {
private:
    UINT32                 _body_sz;       // function body size
    UINT32                 _local_count;   // count of local declarations
//    std::vector<LOCAL> locals;   // dont know what it is
    std::vector<WASM_INST> _expressions;    // code expressions
public:
    FUNCTION(UINT32 bodySz,    UINT32 localCount,        std::vector<WASM_INST> expressions) :
             _body_sz(bodySz), _local_count(localCount), _expressions(std::move(expressions)) {};

    UINT32   getBodySz()                              const { return _body_sz;     }
    UINT32   getLocalCount()                          const { return _local_count; }
    const    std::vector<WASM_INST> &getExpressions() const { return _expressions; }
};


// type section has a sequence of function types
// function type: param type and result type
//
class FUNC_TYPE {
private:
    UINT32 _num_param;
    std::vector<VALTYPE> _type_param;
    UINT32 _num_res;
    std::vector<VALTYPE> _type_res;
public:
    FUNC_TYPE(UINT32 numParam,      std::vector<VALTYPE> typeParam,
              UINT32 numRes,        std::vector<VALTYPE> typeRes) :
              _num_param(numParam), _type_param(std::move(typeParam)),
              _num_res(numRes),     _type_res(std::move(typeRes)) {};

    UINT32  NumParam()                          const { return _num_param;  }
    const   std::vector<VALTYPE> &TypeParam() const { return _type_param; }
    UINT32  NumRes()                            const { return _num_res;    }
    const   std::vector<VALTYPE> &TypeRes()   const { return _type_res;   }
};


// export section has a sequence of exports
//
class EXPORT {
private:
  std::string name;
  UINT16 kind; // func, mem, table, global
  UINT32 index;
public:
    EXPORT(std::string name,      UINT16 kind, UINT32 index) :
           name(std::move(name)), kind(kind),  index(index) {};

    const   std::string &Name() const { return name;  }
    UINT16  Kind()              const { return kind;  }
    UINT32  Index()             const { return index; }
};


// import section has a sequence of imports
//
class IMPORT {
private:
  std::string module_name;
  std::string field_name;
  UINT16 kind; // func, mem, table, global
  UINT32 index;
public:
    IMPORT(std::string moduleName,             std::string fieldName,
           UINT16 kind,                        UINT32 index) :
           module_name(std::move(moduleName)), field_name(std::move(fieldName)),
           kind(kind),                         index(index) {};

    const   std::string &Name() const { return module_name;  }
    UINT16  Kind()              const { return kind;  }
    UINT32  Index()             const { return index; }
};


// function section has a sequence of function signature index
//
class FUNC_SIGN {
private:
    UINT8 index;
public:
    FUNC_SIGN(UINT8 index) : index(index) {};
    UINT8 Index() const { return index; }
};


// instruction table
//
extern WASM_INST WASM_META_INFO[180];

WASM_INST *OPCODE_inst(UINT8 opcode);

typedef std::vector<WASM_INST> WINSVEC;

#endif //WASM2UWASM_WINST_H