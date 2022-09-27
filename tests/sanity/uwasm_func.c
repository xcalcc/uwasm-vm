int foo(int a, int b)
{
  int c;
  c = a + b;
  return c;
}

int bar(int x, int y)
{
  int res;
  res = foo(y, x);
  return res;
}
