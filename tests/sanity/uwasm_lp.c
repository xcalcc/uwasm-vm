int foo(int x, int y)
{
  int res = 0, i = 0;
  for(; i < x; i++)
  {
    res += y - i;
  }
  return res;
}
