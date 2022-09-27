int foo(int a, int b)
{
  int res = 0;
  if(a > b) {
    res = a - b;
  } else {
    res = a + b;
  }
  return res;
}
