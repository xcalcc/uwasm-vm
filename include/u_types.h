/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef U_TYPES_H
#define U_TYPES_H

#ifndef TARG_UWASM
typedef long long                 INT64;
typedef unsigned long long        UINT64;
typedef int                       INT32;
typedef unsigned int              UINT32;
typedef unsigned char             UINT8;
typedef char                      INT8;
typedef unsigned short            UINT16;
typedef short                     INT16;
typedef int                       BOOL;
typedef long	                    INTPTR;
typedef unsigned                  UINTPTR;
#endif
typedef float                     FLOAT32;
typedef double                    FLOAT64;
typedef UINT8 *                   BUFFER;
typedef const UINT8 *             CONST_BUFFER;
#endif //U_TYPES_H
