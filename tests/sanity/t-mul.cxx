template<typename T, int val> T
mul(T a, T b) {
  T c = a * b;
  T d = a * (T) val;
  T e = (T) val * b;
  return c * d * e;
}

int test(int i32a, int i32b, long i64a, long i64b, float f32a, float f32b, double f64a, double f64b) {
  int i32r1 = mul<int, 1>(i32a, i32b);

  long i64r1 = mul<long, 2>(i64a, i64b);

  float f32r1 = mul<float, 3>(f32a, f32b);

  double f64r1 = mul<double, 4>(f64a, f64b);
}
