int sub(int a, int b)
{
  int res = 0;
  res = a - b;
  return res;
}

int foo(int x, int y)
{
  int res = 0;
  if (x > y)
    res = sub(x, y);
  else
    res = sub(y, x);
  return res;
}
