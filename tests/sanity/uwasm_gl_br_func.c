int gbl = 0;

int sub(int a, int b)
{
  int res = 0;
  res = a - b;
  return res;
}

int foo(int x, int y)
{
  if (x > y)
    gbl = sub(x, y);
  else
    gbl = sub(y, x);
  return gbl;
}
