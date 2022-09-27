/*
 * Copyright (C) 2021 Xcalibyte (Shenzhen) Limited.
 */

#include "u_str_tab.h"
#include "trace/u_trace.h"

bool COMPARATOR::operator() (const BUF_LEN_PAIR &p1, const BUF_LEN_PAIR &p2) const {
  bool comp_r = p1.second < p2.second;
  if (p1.second == p2.second) {
    for (UINT32 i = 0; i < p1.second; i++) {
      const char *s1 = p1.first;
      const char *s2 = p2.first;
      if (s1[i] != s2[i]) {
        comp_r = s1[i] < s2[i];
      }
    }
  }
  return comp_r;
}

bool U_STR_TAB::Contains(const char *buf, UINT32 len) {
  pair<const char *, UINT32> buf_pair = make_pair(buf, len);
  return _cache.find(buf_pair) != _cache.end();
}

UINT32 U_STR_TAB::Get_idx(const char *buf, UINT32 len) {
  pair<const char *, UINT32> buf_pair = make_pair(buf, len);
  if (_cache.find(buf_pair) != _cache.end()) {
    return _cache[buf_pair];
  }
  return 0;
}

UINT32 U_STR_TAB::Add(const char *buf, UINT32 len) {
  _entry_vec.emplace_back(make_pair(_raw_buf.size(), len));
  UINT32 entry_idx = _entry_vec.size() - 1;
  UINT32 buf_idx = _raw_buf.size();
  char *pre_addr = _raw_buf.empty() ? NULL : &_raw_buf[0];
  _raw_buf.insert(_raw_buf.end(), buf, buf + len);
  // cache the raw buf
  if (pre_addr == NULL || pre_addr == &_raw_buf[0]) {
    _cache[make_pair(&(_raw_buf[buf_idx]), len)] = entry_idx;
  }
    // the pointer of vector was changed, need to update the cache
  else {
    _cache.clear();
    for (UINT32 i = 0; i < _entry_vec.size(); i++) {
      pair<UINT32, UINT32> p = _entry_vec[i];
      _cache.insert({make_pair(&(_raw_buf[p.first]), p.second), i});
    }
  }
  return entry_idx;
}

UINT32 U_STR_TAB::Get_off(UINT32 idx) {
  Is_True(idx < _entry_vec.size(),
          ("Tab array out of bound, index: %d, vec size: %d.", idx, _entry_vec.size()));
  return _entry_vec[idx].first;
}

UINT32 U_STR_TAB::Request(const char *buf, UINT32 len) {
  pair<const char *, UINT32> buf_pair = make_pair(buf, len);
  // found buf in cache, just return
  if (_cache.find(buf_pair) != _cache.end()) {
    return _cache[buf_pair];
  }
  return Add(buf, len);
}

char *U_STR_TAB::Get_entry_str(UINT32 entry_idx) {
  UINT32 buf_idx = _entry_vec[entry_idx].first;
  return &_raw_buf[buf_idx];
}

void U_STR_TAB::Pad(UINT32 align) {
  if (Get_raw_buf().size() % align != 0) {
    INT32 zero_fill = align - Get_raw_buf().size() % align;
    char *zero_buf = (char *) calloc(zero_fill, 1);
    Add(zero_buf, zero_fill);
  }
}
