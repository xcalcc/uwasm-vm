typedef unsigned long long UINT64;
typedef long long INT64;


INT64 C(int ci1)
{
  int i;
  INT64 j = ci1 + 5;
  for (i=0; i < ci1; i++)
    j++;
  return j;
}

int E(int e1, int e2)
{
  return e1 * e2;
}

INT64 Dcalled_byB(int db1, int db2)
{
  while (db1) {
    db2++;
    db1--;
  }

  return (INT64)db2;
}

UINT64 F(INT64 fl1, int fi2)
{
  return fi2 + fl1;
}

UINT64 B(float bf1, int bi2, int bi3)
{
  double d = bf1;
  int ri, r1;
  if (d != 0.0) {
    int ri = E(bi2, bi3);
  }
  else ri = 0xdeadbeef;

  r1 = d;

  INT64 rlong = C(ri);

  if (rlong > 30) {
    rlong += Dcalled_byB(ri, r1);
  }

  return F(rlong, bi3);

}

UINT64 A(int ai1, float af2)
{
  if (ai1 > 0)
    af2 += 1.0;
  else
    af2 -= 2.0;

  UINT64 l = B(af2, ai1, 3);
  return l;
}
