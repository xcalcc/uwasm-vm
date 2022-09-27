int gbl = 0;

int foo(int x, int y)
{
  if (x > y)
    gbl = x - y;
  else
    gbl = y - x;
  return gbl;
}
