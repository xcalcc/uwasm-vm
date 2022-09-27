/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UBH_H
#define UBH_H

#include <stdio.h>
#include <string.h>
#include "u_am.h"
#include "u_types.h"

#define UBH_MAGIC "uASM"
#define UBH_MAGIC_LEN (4)                            // magic at starttt of uWASM binary file
#define UBH_OTHER_LEN (4)                            // other pertinent info

#define SECTION_ALIGN 8

#define RET_RANGE           2u
#define PARAM_RANGE         4u
#define MAX_TYPE_ELEM_RANGE (32u - RET_RANGE - PARAM_RANGE)
#define MAX_TYPE_ELEM_NUM   (MAX_TYPE_ELEM_RANGE / VALTY_SZ + 1u)
#define TYPE_MASK           ((1u << VALTY_SZ) - 1u)

#define DATA_SEGMENT_MAGIC_PREFIX "UDSv0.1\0\0\0"

enum U_FUNC_FLAG {
  F_EXTERN = 0x0001u,
  F_NATIVE = 0x0002u,
  F_EXPORT = 0x0004u,
  F_IMPORT_WASM = 0x008u,
};

class U_MODULE;

// module header
class UBH_MHDR {
private:
  char           _uMagic[UBH_MAGIC_LEN];
  char           _uOther[UBH_OTHER_LEN];               // this include validation info etc
  UINT32         _shdr_num             : 6;            // the number of shdr
  UINT32         _shdr_off             : 26;           // the offset of section header, use offset find the start of shdr

public:
  UBH_MHDR()                                               { Set_magic(UBH_MAGIC); }
  const char*    Get_umagic()                        const { return _uMagic; }
  const char*    Get_uother()                        const { return _uOther; }
  UINT32         Get_shdr_num()                      const { return _shdr_num; }
  UINT32         Get_shdr_off()                      const { return _shdr_off; }
  void           Set_shdr_off(UINT32 shdr_off)             { _shdr_off = shdr_off; }
  void           Set_shdr_num(UINT32 shdr_num)             { _shdr_num = shdr_num; }
  void           Set_magic(const char data[UBH_MAGIC_LEN]) { strncpy(_uMagic, data, UBH_MAGIC_LEN); }
  void           Set_other(const char data[UBH_OTHER_LEN]) { strncpy(_uOther, data, UBH_OTHER_LEN); }
  void           Print(FILE *file) const;
};

// section tag used for marking the section's kind
typedef enum {
  SEC_CUST,                   // 0x0
  SEC_TYPE,                   // 0x1
  SEC_IMPORT,                 // 0x2
  SEC_FUNC,                   // 0x3
  SEC_TAB,                    // 0x4
  SEC_MEM,                    // 0x5
  SEC_GLBL,                   // 0x6
  SEC_EXPORT,                 // 0x7
  SEC_START,                  // 0x8
  SEC_ELEM,                   // 0x9
  SEC_CODE,                   // 0xA
  SEC_BIN,                    // 0xB
  SEC_DEBUG,                  // 0xC
} SEC_TAG;

// section id used for the sections for
typedef enum {
  S_TYPE,                     // 0x0
  S_IMPORT,                   // 0x1
  S_FUNC,                     // 0x2
  S_DATA,                     // 0x3
  S_GLBL,                     // 0x4
  S_EXPORT,                   // 0x5
  S_EXPORT_GLBL,              // 0x6
  S_CODE,                     // 0x7
  S_NN_STR,                   // 0x8
  S_MODULE_NAME,              // 0x9
  S_GLBL_NAME,                // 0xA
  S_RELOC,                    // 0xB
  S_IMP_NAT,                  // 0xC
  S_NAT,                      // 0xD
  S_COUNT,                    // 0xE
} SECTION_ID;

typedef enum {
  K_MODULE_NAME,
  K_GLBL_NAME,
  K_NON_NULL_STR,
} TAB_SEC_KIND;

typedef enum {
  K_IMPORT_MODULE,
  K_IMPORT_NATIVE,
} IMPORT_SEC_KIND;

typedef enum {
  K_EXPORT,
  K_EXPORT_GLBL,
} EXPORT_SEC_KIND;

// import and export entry kind
typedef enum {
  K_FUNC,
  K_TAB,
  K_MEM,
  K_GLBL,
} IE_ENTRY_KIND;

// Section header
class UBH_SHDR {
private:
  union {
    struct {
      UINT16     _sec_tag              : 4;
      UINT16     _kind                 : 12;
      UINT16     _elem_num             : 16;           // number of element
    } S1;
    struct {
      UINT16     _sec_tag              : 4;
      UINT16     _kind                 : 4;
      UINT32     _elem_num             : 24;           // number of element
    } S2;
    struct {
      UINT16     _sec_tag              : 4;
      UINT32     _dummy                : 28;
    } S3;
  } U;
  UINT32         _idx                  : 32;           // index to section vector, must be access through an access function
  UINT8          Select_struct() const;

public:
  explicit UBH_SHDR(UINT16 sec_tag, UINT16 elem_num, UINT32 idx);
  UINT16         Get_sec_tag()                       const { return U.S1._sec_tag; }
  UINT32         Get_elem_num() const;
  UINT32         Get_idx()                           const { return _idx; }
  UINT16         Get_kind() const;
  void           Set_sec_tag(UINT16 sec_tag)               { U.S1._sec_tag = sec_tag; /* just set s1 */ }
  void           Set_elem_num(UINT16 elem_num);
  void           Set_idx(UINT32 elem_num)                  { _idx = elem_num; }
  void           Set_kind(UINT16 kind);
  bool           Have_sec() const;
  void           Print(FILE *file);
};

class UBH_TYPE {
  friend class U_MODULE;
private:
  UINT16         _ret_num             : RET_RANGE;
  UINT16         _param_num           : PARAM_RANGE;
  UINT32         _elem                : MAX_TYPE_ELEM_RANGE;    // param/ret valty if < 14, else index to non null string

private:
  UINT8          Get_param_num()                     const { return _param_num; }   // this method just can be used in UBH_MODULE

public:
  UBH_TYPE(UINT16 ret_num, UINT16 param_num, UINT32 elem) : _ret_num(ret_num), _param_num(param_num), _elem(elem) {}
  UINT8          Get_ret_num()                       const { return _ret_num; }
  UINT32         Get_elem()                          const { return _elem; }
  BOOL           Is_stored_local()                   const { return _param_num != ((0x01u << PARAM_RANGE) - 1); }
  bool           operator== (UBH_TYPE type) const;
  void           Print(FILE *file, const U_MODULE &module) const;
};

class UBH_IMPORT {
private:
  UINT32         _desc_tag             : 4;            // func: 0, table: 1, mem:2, global: 3
  UINT32         _func_idx             : 28;

public:
  explicit  UBH_IMPORT(UINT8 desc_tag, UINT32 func_idx) : _desc_tag(desc_tag), _func_idx(func_idx) {}
  UINT8          Get_desc_tag()                      const { return _desc_tag; }
  UINT32         Get_func_idx()                      const { return _func_idx; }
  void           Print(FILE *file) const;
};

class UBH_IMP_NAT {
private:
  UINT32         _desc_tag             : 4;            // func: 0, table: 1, mem:2, global: 3
  UINT32         _func_idx             : 28;
  UINT32         _ntype_off            : 32;           // native type offset, find native type in non-null string tab
  UINT32         _nat_idx              : 8;            // native entry index
  UINT32         _nat_off              : 24;           // native entry buffer offset

public:
  explicit  UBH_IMP_NAT(UINT8 desc_tag, UINT32 func_idx, UINT32 ntype_off) :
    _desc_tag(desc_tag), _func_idx(func_idx), _ntype_off(ntype_off), _nat_off(0xffffff) {}
  void           Set_nat_idx(UINT32 nat_idx)               { _nat_idx = nat_idx; }
  void           Set_nat_off(UINT32 nat_off)               { _nat_off = nat_off; }
  UINT8          Get_desc_tag()                      const { return _desc_tag; }
  UINT32         Get_func_idx()                      const { return _func_idx; }
  UINT32         Get_ntype_off()                     const { return _ntype_off; }
  UINT32         Get_nat_idx()                       const { return _nat_idx; }
  UINT32         Get_nat_off()                       const { return _nat_off; }
  void           Print(FILE *file) const;
};

class UBH_GLBL {
private:
  UINT16         _mut                  : 4;           // 0: const, 1: var
  UINT16         _ty                   : 2;           // global valtype, int or float
  UINT32         _dummy                : 26;
  UINT32         _data_idx             : 32;          // valtype if const, else index to data section
  UINT64         _init_val             : 64;
public:
  UINT8          Is_mut()                            const { return _mut; }
  UINT16         Get_ty()                            const { return _ty; }
  void           Set_mutable(bool mut)                     { _mut = mut; }
  void           Set_ty(UINT32 ty)                         { _ty = ty;   }
  void           Set_init_val(UINT64 i)                    { _init_val = i; }
  UINT64         Get_init_val()                      const { return _init_val; }
  UINT32         Get_data_idx()                      const { return _data_idx; }
  void           Set_data_idx(UINT32 data_idx)             { _data_idx = data_idx; }
  void           Set_dummy()                               { _dummy = 0; }
  void           Print(FILE *file = stderr) const;
};

// Export
class UBH_EXPORT {
private:
  UINT32         _desc_tag             : 4;           // func: 0, table: 1, mem:2, global: 3
  UINT32         _func_idx             : 28;          // just support export function here
public:
  explicit  UBH_EXPORT(UINT8 desc_tag, UINT32 func_idx) : _desc_tag(desc_tag), _func_idx(func_idx) {}
  UINT16         Get_desc_tag()                      const { return _desc_tag; }
  UINT32         Get_func_idx()                      const { return _func_idx; }
  void           Print(FILE *file) const;
};

class UBH_EXP_GLBL {
private:
  UINT32         _glbl_name_idx;
  UINT32         _glbl_tab_idx;

public:
  UINT32         Get_glbl_name_idx()                 const { return _glbl_name_idx; }
  UINT32         Get_glbl_tab_idx()                  const { return _glbl_tab_idx; }
};

// UBH Function instance
class UBH_FUNC {
private:
  UINT16         _type_idx             : 16;           // index to type table
  UINT16         _name_idx             : 16;           // index to function name string table
  UINT16         _flags                : 16;
  UINT16         _code_idx             : 16;

public:
  UBH_FUNC(const UINT16 type_idx, const UINT16 name_idx, UINT16 flags) :
    _type_idx(type_idx), _name_idx(name_idx), _flags(flags), _code_idx(0) {
  }
  UBH_FUNC(const UINT16 type_idx, const UINT16 name_idx, UINT16 flags, UINT32 code_idx) :
    _type_idx(type_idx), _name_idx(name_idx), _flags(flags), _code_idx(code_idx) {
  }
  void           Set_code_idx(UINT16 code_idx)             { _code_idx = code_idx; }
  UINT32         Get_type_idx()                      const { return _type_idx; }
  UINT32         Get_name_idx()                      const { return _name_idx; }
  UINT32         Get_flags()                         const { return _flags; }
  UINT32         Get_code_idx() const;
  BOOL           Is_set_flag(UINT16 flag)            const { return _flags & flag; }
  void           Print(FILE *file) const;
};

/*
 * part of data structure contains variable length field, to make it more convenient to use mmap
 * we use offset instead of pointer, the offset is based on the start of the section address
 * we will fixup those offsets after decode, user can get address by using api
 */
class UBH_CODE {
private:
  UINT32         _i32_local            : 10;           // i32 local num
  UINT32         _i64_local            : 10;
  UINT32         _f32_local            : 10;
  UINT32         _dummy                : 2;

  UINT32         _f64_local            : 10;
  UINT32         _stack_size           : 22;           // local stack size

  UINT8          _i32_io               : 8;            // i32 max call param num
  UINT8          _i64_io               : 8;
  UINT8          _f32_io               : 8;
  UINT8          _f64_io               : 8;

  UINT32         _label_num            : 12;           // total number of labels of this function
  UINT32         _code_size            : 20;           // code size, how many number of 32 bits
  UINT32         _inst_tab_checksum    : 32;
  UINT32         _label_tab_checksum   : 32;
  UINT64         _inst_tab_off         : 64;
  UINT64         _label_tab_off        : 64;
  // char       *_inst_tab;
  // char       *_label_tab;

public:
  UBH_CODE (UINT32 local_regs[4], UINT32 io_regs[4], UINT32 stack_size) :
    _i32_local(local_regs[0]), _i64_local(local_regs[1]), _f32_local(local_regs[2]), _dummy(0), _f64_local(local_regs[3]),
    _stack_size(stack_size), _i32_io(io_regs[0]), _i64_io(io_regs[1]), _f32_io(io_regs[2]), _f64_io(io_regs[3]),
    _label_num(0), _code_size(0), _label_tab_off(0), _label_tab_checksum(0), _inst_tab_off(0), _inst_tab_checksum(0) {}
  void           Set_label_num(UINT32 label_num)           { _label_num = label_num; }
  void           Set_code_size(UINT32 code_size)           { _code_size = code_size; }
  void           Set_label_tab_off(UINT64 off)             { _label_tab_off = off; }
  void           Set_label_tab(UINT32 *label_tab)          { _label_tab_off = (UINT64) label_tab; }
  void           Set_inst_tab_off(UINT64 off)              { _inst_tab_off = off; }
  void           Set_inst_tab(UINT32 *inst_tab)            { _inst_tab_off = (UINT64) inst_tab; }
  UINT32         Get_i32_local()                     const { return _i32_local; }
  UINT32         Get_i64_local()                     const { return _i64_local; }
  UINT32         Get_f32_local()                     const { return _f32_local; }
  UINT32         Get_f64_local()                     const { return _f64_local; }
  UINT32         Get_stack_size()                    const { return _stack_size; }
  UINT32         Get_i32_io()                        const { return _i32_io; }
  UINT32         Get_i64_io()                        const { return _i64_io; }
  UINT32         Get_f32_io()                        const { return _f32_io; }
  UINT32         Get_f64_io()                        const { return _f64_io; }
  UINT32         Get_label_num()                     const { return _label_num; }
  UINT32         Get_code_size()                     const { return _code_size; }
  UINT64         Get_label_tab_off()                 const { return _label_tab_off; }         // only can call before fixup
  UINT64         Get_inst_lab_off()                  const { return _inst_tab_off; }          // only can call before fixup
  UINT32        *Get_label_tab()                     const { return (UINT32 *) _label_tab_off; }
  UINT32        *Get_inst_tab()                      const { return (UINT32 *) _inst_tab_off; }
  void           Print(FILE *file)                   const;
  void           Update_ubh_code(const UINT32 *local_regs,
                                 const UINT32 *io_regs,
                                 const UINT32 stack_size) {
    _i32_local          = local_regs[0];
    _i64_local          = local_regs[1];
    _f32_local          = local_regs[2];
    _f64_local          = local_regs[3];
    _stack_size         = stack_size;
    _i32_io             = io_regs[0];
    _i64_io             = io_regs[1];
    _f32_io             = io_regs[2];
    _f64_io             = io_regs[3];
  }
};

class UBH_NAT {
private:
  UINT32         _byte_num      : 32;
  UINT32         _checksum      : 32;
  UINT64         _buf_off       : 64;
  // char *      _buf                              // hidden field, field _buf_off should be fixup to this pointer

public:
  UBH_NAT(UINT32 byte_num, UINT64 buf_off, UINT32 checksum) : _byte_num(byte_num), _buf_off(buf_off), _checksum(checksum) {}
  UINT32         Byte_number()                       const { return _byte_num; }
  UINT64         Buf_offset()                        const { return _buf_off; }
  char          *Get_buf()                                 { return (char *) _buf_off; }
  void           Set_buf(char *buf)                        { _buf_off = (UINT64) buf; }
  void           Set_buf_off(UINT64 buf_off)               { _buf_off = buf_off; }
};

typedef enum {
  MEM_DATA,
  MEM_RELOC,
} MEM_SEC_KIND;

// Memory
class UBH_MEM {
private:
  UINT32         _ty                   : 4;           // 0 = min, 1 = min + max
  UINT32         _min_limit            : 28;          // min value
  UINT32         _max_limit            : 32;          // max value
public:
  UINT32         Get_type()                          const { return _ty; }
  UINT32         Get_min_limit()                     const { return _min_limit; }
  UINT32         Get_max_limit()                     const { return _max_limit; }
  void           Set_min_limit(UINT32 min_limit)           { _min_limit = min_limit; }
  void           Set_max_limit(UINT32 max_limit)           { _max_limit = max_limit; }
  void           Set_ty(UINT32 ty)                         { _ty = ty; }
};

// Data structure for describing the data segments as in WASM.
class UBH_DSEG_INFO {
  UINT32 _mem_offset;
  UINT32 _size;
public:
  UBH_DSEG_INFO(): _mem_offset(0), _size(0) {}
  UBH_DSEG_INFO(UINT32 mem_ofs, UINT32 size): _mem_offset(mem_ofs), _size(size) {}
  UINT32 Get_mem_ofs()       const { return _mem_offset; }
  UINT32 Get_size()          const { return _size; }
  void Set_size(UINT32 sz)         { _size = sz; }
  void Set_mem_ofs(UINT32 mem_ofs) { _mem_offset = mem_ofs; }
};

class UBH_DATA_SEGMENTS {
  UINT8         _magicNumber[8];
  UINT64        _elements;
  UBH_DSEG_INFO  _infos[0];
public:
  UBH_DATA_SEGMENTS(): _magicNumber(), _elements(0), _infos{}
  {
    const char  *magic = DATA_SEGMENT_MAGIC_PREFIX;
    *(UINT64 *) _magicNumber = *(UINT64 *) magic;
  };
  UINT32 Get_size()                  const { return _elements; }
  void   Set_size(UINT32 elts)             { _elements = elts; }
  UINT32 Calc_size()                 const { return _elements * sizeof(UBH_DSEG_INFO) + sizeof(UBH_DATA_SEGMENTS); }
  UINT32 Calc_size(UINT32 elts)      const { return elts * sizeof(UBH_DSEG_INFO) + sizeof(UBH_DATA_SEGMENTS);      }
  UBH_DSEG_INFO &Get_seg(UINT32 idx)       { return _infos[idx];}
  UINT32 Get_seg_ofs(UINT32 idx)           { return _infos[idx].Get_mem_ofs();}
  UINT32 Get_seg_size(UINT32 idx)          { return _infos[idx].Get_size();}
  UINT32 Calc_total_space()          const {
    UINT32 total = Calc_size();
    for (UINT32 i = 0; i < _elements; i++) {
      total += _infos[i].Get_size();
    }
    return total;
  }
  UINT32 Calc_max_ofs() const {
    UINT32 max = 0;
    for (UINT32 i = 0; i < _elements; i++) {
      if (max < _infos[i].Get_size() + _infos[i].Get_mem_ofs()) {
        max = _infos[i].Get_size() + _infos[i].Get_mem_ofs();
      }
    }
    return max;
  }
};

BOOL Check_ubh_ds_size();

#endif // UBH_H
