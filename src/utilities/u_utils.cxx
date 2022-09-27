/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include <errno.h>
#include <stdlib.h>

#include "u_utils.h"
#include "trace/u_trace.h"

INT32 Read_and_mmap(const char *file_path, char **buffer, UINT32 *buffer_len) {
  INT32 fd;
  struct stat statbuf;
  /* open the input file */
  // need to fix up relocatable symbol offset, need write permission
  fd = open(file_path, O_RDONLY);
  if (fd < 0) {
    Is_True(false, ("can't open %s for reading", file_path));
    return fd;
  }
  /* find size of input file */
  if (fstat(fd, &statbuf) < 0) {
    Is_True(false, ("fstat error"));
    return -1;
  }
  /* mmap the input file */
  *buffer = (char *) mmap (NULL, statbuf.st_size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
  if ((*buffer) == MAP_FAILED) {
    Is_True(false, ("mmap error for input, errno: %d.", errno));
    return -1;
  }
  close(fd);
  *buffer_len = (UINT32) statbuf.st_size;
  /* this copies the input file to the output file */
  return fd;
}

INT32 Write_and_mmap(const char *file_path, UINT32 buffer_len, char **buffer) {
  INT32 fd;
  /* open/create the output file */
  fd = open(file_path, O_RDWR|O_CREAT|O_TRUNC, 0666);
  if (fd < 0) {
    Is_True (false, ("can't create %s for writing, %d, errno = %d", file_path, fd, errno));
    return fd;
  }
  /* go to the location corresponding to the last byte */
  if (lseek(fd, buffer_len - 1, SEEK_SET) == -1) {
    Is_True(false, ("lseek error"));
    return -1;
  }
  /* write a dummy byte at the last location */
  if (write(fd, "", 1) != 1) {
    Is_True(false, ("write error"));
    return -1;
  }
  /* mmap the output file */
  *buffer = (char *) mmap(NULL, buffer_len, PROT_READ|PROT_WRITE,
                          MAP_SHARED, fd, 0);
  if ((*buffer) == (char *) -1) {
    Is_True(false, ("mmap error for output, errno: %d.", errno));
    return -1;
  }
  return fd;
}

INT32 Finish_write(INT32 fd, char *buf, UINT32 buf_len) {
  INT32 ret = msync(buf, buf_len, MS_SYNC);
  Is_True(ret == 0, ("Call msync failed, return value: %d.", ret));
  if (ret != 0) {
    return ret;
  }
  ret = close(fd);
  Is_True(ret == 0, ("Close fd filed, fd: %d, errno: %d.", fd, errno));
  munmap(buf, buf_len);
  return 0;
}
