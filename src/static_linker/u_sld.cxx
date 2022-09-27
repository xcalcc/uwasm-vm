/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <stdio.h>
#include <stdlib.h>
#include "u_sld.h"
#include "trace/u_trace.h"
#include "utilities/u_utils.h"
#include "ubh/ubh.h"
#include <iostream>
#include <string.h>

#define TEXT_ALIGN 32
#define OUTFILE_EXT ".sld"

using namespace std;
using namespace ELFIO;

static bool trace = false;

bool STR_COMPARATOR::operator() (const char *s1, const char *s2) const {
  return strcmp(s1, s2) < 0;
}

U_SLD::U_SLD() : _text_sec(nullptr) {
}

void U_SLD::Open_ubh_file(const char *file_path) {
  Read_and_mmap(file_path, &_buf, &_buf_len);
  _u_module.Set_keep_off(true);
  U_MODULE::Init_module_from_buffer(&_u_module, _buf, _buf_len);
  if (!_u_module.Contains_section(S_IMP_NAT)) {
     return;
  }
  Is_Trace(Tracing(TP_SLD), (TFile, DBAR));
  Is_Trace(Tracing(TP_SLD), (TFile, "Add import native function to cache.\n"));
  Is_Trace(Tracing(TP_SLD), (TFile, DBAR));
  UBH_SHDR &imp_nat_shdr = _u_module.Get_shdr_by_sec_id(S_IMP_NAT);
  for (UINT32 i = 0; i < imp_nat_shdr.Get_elem_num(); i++) {
    UBH_IMP_NAT &imp_nat_entry = _u_module.Get_imp_nat(i);
    UINT32 func_idx = imp_nat_entry.Get_func_idx();
    UBH_FUNC &func = _u_module.Get_func(func_idx);
    const char *func_name = _u_module.Get_module_name(func.Get_name_idx());
    _name_import_cache[func_name] = i;
    Is_Trace(Tracing(TP_SLD), (TFile, "import native func: %s\n", func_name));
  }
}

void U_SLD::Open_so_file(const char *file_path) {
  bool result = _reader.load(string(file_path));
  if (!result) {
    Is_Trace(Tracing(TP_SLD), (TFile, "Read elf file error."));
    exit(-1);
  }
  UINT32 sec_num = _reader.sections.size();
  for (UINT32 i = 0; i < sec_num; i++) {
    section *psec = _reader.sections[i];
    if (psec->get_type() == SHT_PROGBITS && strcmp(psec->get_name().c_str(), ".text") == 0) {
      _text_sec = psec;
    }
  }
  Is_Trace(Tracing(TP_SLD), (TFile, DBAR));
  Is_Trace(Tracing(TP_SLD), (TFile, "Find native in elf file.\n"));
  Is_Trace(Tracing(TP_SLD), (TFile, DBAR));
  for (UINT32 i = 0; i < sec_num; i++) {
    section *psec = _reader.sections[i];
    // symbol table
    if (psec->get_type() == SHT_SYMTAB) {
      symbol_section_accessor symbols(_reader, psec);
      Elf_Xword sym_no = symbols.get_symbols_num();
      if (sym_no == 0) {
        return;
      }
      for (Elf_Xword i = 0; i < sym_no; ++i) {
        std::string name;
        Elf64_Addr value = 0;
        Elf_Xword size = 0;
        unsigned char bind = 0;
        unsigned char type = 0;
        Elf_Half section = 0;
        unsigned char other = 0;
        symbols.get_symbol(i, name, value, size, bind, type,
                            section, other);
        if (type != STT_FUNC) {
          continue;
        }
        const char *name_c_str = name.c_str();
        if (_name_import_cache.find(name_c_str) != _name_import_cache.end()) {
          UBH_IMP_NAT &import_native_entry = _u_module.Get_imp_nat(_name_import_cache[name_c_str]);
          import_native_entry.Set_nat_idx(0);
          import_native_entry.Set_nat_off(value - _text_sec->get_offset());
          Is_Trace(Tracing(TP_SLD), (TFile, "matched native func: %s, native section offset: %lu\n",
                                  name_c_str, value - _text_sec->get_offset()));
        } else {
          Is_Trace(Tracing(TP_SLD), (TFile, "not mached native func: %s\n", name_c_str));
        }
      }
    }
    // text section
  }
}

void U_SLD::Link(char *output_path) {
  UINT64 nw_buf_size = _buf_len;
  UINT32 aligned_ts_size = _text_sec->get_size();
  if (aligned_ts_size % SECTION_ALIGN != 0) {
    UINT32 dim  = aligned_ts_size % SECTION_ALIGN;
    aligned_ts_size += SECTION_ALIGN - dim;
  }
  nw_buf_size += sizeof(UBH_NAT);
  nw_buf_size += aligned_ts_size;
  // new shdr should be added
  nw_buf_size += sizeof(UBH_SHDR);
  char *nw_buf = NULL;
  UINT32 fd = Write_and_mmap(output_path, nw_buf_size, &nw_buf);
  char *nw_buf_p = nw_buf;
  UINT32 shdr_num = _u_module.Mhdr().Get_shdr_num();
  UINT64 shdr_off = _buf_len - sizeof(UBH_SHDR) * shdr_num;
  // copy the old buffer to the new buffer, leave shdr alone, will copy them later
  memcpy(nw_buf_p, _buf, shdr_off);
  nw_buf_p += shdr_off;
  // set content to zero and copy text to new buffer
  memset(nw_buf_p, 0, nw_buf_size - shdr_off);
  // create binary entry, and copy
  UBH_NAT binary_entry(aligned_ts_size, sizeof(UBH_NAT), 0);
  memcpy(nw_buf_p, &binary_entry, sizeof(UBH_NAT));
  nw_buf_p += sizeof(UBH_NAT);
  memcpy(nw_buf_p, _text_sec->get_data(), _text_sec->get_size());
  nw_buf_p += aligned_ts_size;
  // copy section header to new buffer
  UINT32 nw_shdr_off = shdr_off + sizeof(UBH_NAT) + aligned_ts_size;
  memcpy(nw_buf_p, _u_module.Shdr_tab(), shdr_num * sizeof(UBH_SHDR));
  UBH_SHDR bin_shdr(SEC_BIN, 1, shdr_off);
  memcpy(nw_buf_p + shdr_num * sizeof(UBH_SHDR), &bin_shdr, sizeof(UBH_SHDR));
  UBH_MHDR *mhdr = (UBH_MHDR *) nw_buf;
  mhdr->Set_shdr_off(nw_shdr_off);
  mhdr->Set_shdr_num(mhdr->Get_shdr_num() + 1);
  char *nw_buf_cp = (char *) malloc(nw_buf_size);
  memcpy(nw_buf_cp, nw_buf, nw_buf_size);
  Finish_write(fd, nw_buf, nw_buf_size);
//  U_MODULE nw_module;
//  nw_module.Set_keep_off(true);
//  U_MODULE::Init_module_from_buffer(&nw_module, nw_buf_cp, nw_buf_size);
//  nw_module.Print();
}

/*
 * Usage: u_slink f1.uwm f2.so
 */
int main(int argc, char **argv) {
  if (argc < 3) {
    fprintf(stderr, "Argument error, usage: u_slink f1.uwm f2.so\n");
    exit(-1);
  }
  if (argc > 3 && strcmp(argv[3], "-t") == 0) {
    Set_tracing_opt(TP_SLD, TRACE_ENABLE_LEVEL);
  }
  int i = 0;
  char *output_file = NULL;
  if (argc > 3) {
    for (i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-o") == 0) {
        if (i + 1 >= argc) {
          fprintf(stderr, "Input commond error, please specific output file name following '-o' option.");
          exit(-1);
        }
        output_file = argv[i + 1];
        break;
      }
    }
  }
  char *uwm_file = i == 1 ? argv[3] : argv[1];
  char *elf_file = i == 1 ? argv[4] : argv[2];
  U_SLD sld;
  sld.Open_ubh_file(uwm_file);
  sld.Open_so_file(elf_file);
  if (output_file == NULL) {
    output_file = (char *) malloc(strlen(uwm_file) + 1 + strlen(OUTFILE_EXT));
    strcpy(output_file, uwm_file);
    strcat(output_file, OUTFILE_EXT);
  }
  sld.Link(output_file);
}
