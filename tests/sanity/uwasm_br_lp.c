int foo(int a, int b)
{
  int res = 0, i = 0;
  if (a > 0) {
    for (; i < a; i++)
      res += b;
  }
  else {
    res = b;
  }
  return res;
}
