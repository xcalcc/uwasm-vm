/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_SLD_H
#define UWASM_VM_U_SLD_H

#include <map>
#include "u_types.h"
#include "ubh/u_module.h"
#include "elfio/elfio.hpp"

class STR_COMPARATOR {
public:
  bool operator() (const char *s1, const char *s2) const;
};

typedef std::map<const char *, UINT32, STR_COMPARATOR> STR_UINT32_MAP;

class U_SLD {
private:
  // just allowed open 1 ubh file
  U_MODULE _u_module;
  ELFIO::elfio _reader;
  char *_buf;
  UINT32 _buf_len;
  ELFIO::section *_text_sec;
  STR_UINT32_MAP        _name_import_cache;

public:
  U_SLD();
  void Open_ubh_file(const char *file_path);
  void Open_so_file(const char *file_path);
  void Link(char *output_path);
};

#endif //UWASM_VM_U_SLD_H
