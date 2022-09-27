template<typename T1, typename T2> 
T2 conv(T1 a) {
  return a;
}

int test(char i8, short i16, int i32, long i64, float f32, double d64) {

  char     char2char     = conv<char, char>(i8);
  short    char2short    = conv<char, short>(i8);
  int      char2int      = conv<char, int>(i8);
  long     char2long     = conv<char, long>(i8);
  float    char2float    = conv<char, float>(i8);
  double   char2double   = conv<char, double>(i8);

  char     short2char    = conv<short, char>(i16);
  short    short2short   = conv<short, short>(i16);
  int      short2int     = conv<short, int>(i16);
  long     short2long    = conv<short, long>(i16);
  float    short2float   = conv<short, float>(i16);
  double   short2double  = conv<short, double>(i16);

  char     int2char      = conv<int, char>(i32);
  short    int2short     = conv<int, short>(i32);
  int      int2int       = conv<int, int>(i32);
  long     int2long      = conv<int, long>(i32);
  float    int2float     = conv<int, float>(i32);
  double   int2double    = conv<int, double>(i32);

  char     long2char     = conv<long, char>(i64);
  short    long2short    = conv<long, short>(i64);
  long     long2int      = conv<long, int>(i64);
  long     long2long     = conv<long, long>(i64);
  float    long2float    = conv<long, float>(i64);
  double   long2double   = conv<long, double>(i64);

  char     float2char    = conv<float, char>(f32);
  short    float2short   = conv<float, short>(f32);
  int      float2int     = conv<float, int>(f32);
  long     float2long    = conv<float, long>(f32);
  float    float2float   = conv<float, float>(f32);
  double   float2double  = conv<float, double>(f32);

  char     double2char   = conv<double, char>(d64);
  short    double2short  = conv<double, short>(d64);
  int      double2int    = conv<double, int>(d64);
  long     double2long   = conv<double, long>(d64);
  double   double2float = conv<double, float>(d64);
  double   double2double = conv<double, double>(d64);
 
  return 0;
}
