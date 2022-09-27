/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef UWASM_VM_U_UTILS_H
#define UWASM_VM_U_UTILS_H
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "u_types.h"

enum IO_STATE {
  IO_OK    = 0,
  IO_ERROR = 1,
};

INT32 Read_and_mmap(const char *file_path, char **buffer, UINT32 *buffer_len);
INT32 Write_and_mmap(const char *file_path, UINT32 buffer_len, char **buffer);
INT32 Finish_write(INT32 fd, char *buf, UINT32 buf_len);

#endif //UWASM_VM_U_UTILS_H
