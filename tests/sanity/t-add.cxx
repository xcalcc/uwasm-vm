template<typename T, int val> T
add(T a, T b) {
  T c = a + b;
  T d = a + val;
  T e = val + b;
  return c + d + e;
}

int test(int i32a, int i32b, long i64a, long i64b, float f32a, float f32b, double f64a, double f64b) {
  int i32r1 = add<int, 1>(i32a, i32b);

  long i64r1 = add<long, 2>(i64a, i64b);

  float f32r1 = add<float, 3>(f32a, f32b);

  double f64r1 = add<double, 4>(f64a, f64b);
}
