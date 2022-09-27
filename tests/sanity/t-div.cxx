int test(int i32a, int i32b, long i64a, long i64b, float f32a, float f32b, double f64a, double f64b);

int main()
{
  test(1, 2, 3, 4, 1.0, 2.0, 3.0, 4.0);
}

template<typename T1, typename T2, typename T3>  T3
div(T1 a, T2 b) {
  return a / b;
}

int test(int i32a, int i32b, long i64a, long i64b, float f32a, float f32b, double f64a, double f64b) {
  
  // same type div
  int i32r1 = div<int, int, int>(i32a, i32b);

  long i64r1 = div<long, long, long>(i64a, i64b);
  
  float f32r1 = div<float, float, float>(f32a, f32b);
  
  double f64r1 = div<double, double, double>(f64a, f64b);

  // mixed type div
  long i64i32r1 = div<long, int, long>(i64a, i32a);
  float i64f32r1 = div<long, float, float>(i64a, f32a);
  double i64f64r1 = div<long, double, double>(i64a, f64a);

  long i32i64r1 = div<int, long, long>(i32a, i64a);
  float i32f32r1 = div<int,  float, float>(i32a, f32a);
  double i32f64r1 = div<int,  double, double>(i32a, f64a);

  float f32i32r1 = div<float, int, float>(f32a, i32a);
  float f32i64r1 = div<float, long, float>(f32a, i64a);
  double f32f64r1 = div<float, double, double>(f32a, f64a);

  double f64i32r1 = div<double, int, double>(f64a, i32a);
  double f64i64r1 = div<double, long, double>(f64a, i64a);
  double f64f32r1 = div<double, float, double>(f64a, f32a);

  return 0;
}

