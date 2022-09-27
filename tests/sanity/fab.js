function fab(a) {
  a = 0|a;   // this is needed so far to get a’s type right
  if (a <= 1)
    return 1;
  return 0 + fab(a-1) + fab(a-2);
}
 
fab(45);

