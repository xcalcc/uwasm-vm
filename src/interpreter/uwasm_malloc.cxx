/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <stdlib.h>
#include <assert.h>
#include "uwasm_malloc.h"

#ifndef UWASM_VM
#define UWASM_VM
#endif /* UWASM_VM */

#define DLMALLOC_EXPORT static
#define ONLY_MSPACES 1
#define USE_DL_PREFIX
#define USE_LOCKS 0
#define NO_MALLINFO 1
#define NO_MALLOC_STATS 1
#define MMAP(s) UVM_mmap(s)

static inline void *
UVM_mmap(size_t) {
  assert(0 && "UVM_mmap called");
  return NULL;
}

#include "thirdparty/dlmalloc/dlmalloc.h"

#define MSPACE_LOCKED 0

static mspace
UVM_pbase_to_space(void *pbase) {
  mstate m = (mstate)(chunk2mem(align_as_chunk((char*)pbase)));
  assert(m->extp == pbase && "mstate extp pointer mismatch");
  return (mspace)m;
}

void
UVM_heap_initialize(void *pbase, size_t size) {
  mspace m = create_mspace_with_base(pbase, size, MSPACE_LOCKED);
  assert(((mstate)m)->extp == pbase && "mstate extp pointer mismatch");
  assert(((mstate)m)->exts == size && "mstate extp pointer mismatch");
}

void
UVM_heap_finalize(void *pbase) {
  mspace m = UVM_pbase_to_space(pbase);
  destroy_mspace(m);
}

uvm_ptr_t
UVM_heap_malloc(void *pbase, size_t size) {
  mspace m = UVM_pbase_to_space(pbase);
  size_t mem = (char*) mspace_malloc(m, size) - (char*) pbase;
  assert(mem < ((mstate)m)->exts && "malloc failed");
  return (uvm_ptr_t) mem;
}

uvm_ptr_t
UVM_heap_realloc(void *pbase, uvm_ptr_t mem, size_t size) {
  mspace m = UVM_pbase_to_space(pbase);
  char *oldp = (char*)pbase + mem;
  size_t newm = (char*)mspace_realloc(m, oldp, size) - (char*)pbase;
  assert(newm < ((mstate)m)->exts && "realloc failed");
  return (uvm_ptr_t)newm;
}

uvm_ptr_t
UVM_heap_clloc(void *pbase, size_t memb, size_t size) {
  mspace m = UVM_pbase_to_space(pbase);
  size_t mem = (char*)mspace_calloc(m, memb, size) - (char*)pbase;
  assert(mem < ((mstate)m)->exts && "calloc failed");
  return (uvm_ptr_t)mem;
}

void
UVM_heap_free(void *pbase, uvm_ptr_t mem) {
  mspace m = UVM_pbase_to_space(pbase);
  assert((size_t)mem < ((mstate)m)->exts && "free invalid pointer");
  void *ptr = (void*)((char*)pbase + mem);
  mspace_free(m, ptr);
}

