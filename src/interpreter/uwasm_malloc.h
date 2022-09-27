/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_MALLOC_H
#define UWASM_MALLOC_H

#include <stddef.h>

typedef int32_t uvm_ptr_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void      UVM_heap_initialize(void *pbase, size_t size);
void      UVM_heap_finalize(void *pbase);
uvm_ptr_t UVM_heap_malloc(void *pbase, size_t size);
uvm_ptr_t UVM_heap_realloc(void *pbase, uvm_ptr_t mem, size_t size);
uvm_ptr_t UVM_heap_calloc(void *pbase, size_t memb, size_t size);
void      UVM_heap_free(void *pbase, uvm_ptr_t mem);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* UWASM_MALLOC_H */
