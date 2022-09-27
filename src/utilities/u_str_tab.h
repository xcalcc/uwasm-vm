/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#ifndef U_STR_TAB_H
#define U_STR_TAB_H

#include <vector>
#include <cstring>
#include <map>
#include <stdlib.h>
#include "u_types.h"

using namespace std;
typedef pair<const char *, UINT32> BUF_LEN_PAIR;

class COMPARATOR {
public:
  bool operator() (const BUF_LEN_PAIR &p1, const BUF_LEN_PAIR &p2) const;
};

typedef map<BUF_LEN_PAIR, UINT32, COMPARATOR> STR_TAB_CACHE;

class U_STR_TAB {
public:
  vector<pair<UINT32, UINT32> >      _entry_vec;       // saved string entry, pair<raw buf index, length>
  vector<char>                       _raw_buf;
  STR_TAB_CACHE                      _cache;           // cache string entry, value is entry vec index, can accelerate finding speed

public:
  bool                  Contains(const char *buf, UINT32 len);
  bool                  Contains(const char *str)                  { return Contains(str, strlen(str) + 1); }
  UINT32                Get_idx(const char *buf, UINT32 len);
  UINT32                Get_idx(const char *str)                   { return Get_idx(str, strlen(str) + 1); }
  UINT32                Add(const char *buf, UINT32 len);
  UINT32                Add(const char *str)                       { return Add(str, strlen(str) + 1); }
  UINT32                Get_off(UINT32 idx);
  UINT32                Request(const char *buf, UINT32 len);
  UINT32                Request(const char *str)                   { return Request(str, strlen(str) + 1); }
  char                 *Get_entry_str(UINT32 entry_idx);
  vector<char>         &Get_raw_buf()                              { return _raw_buf; }
  vector<pair<UINT32, UINT32> > Get_entry_vec()              const { return _entry_vec; }
  void                  Pad(UINT32 align);
};

#endif //U_STR_TAB_H
