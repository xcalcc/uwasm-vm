int foo(int a, int b) {
  return a & b;
}

int main() {
  return foo(0xFF, 0xDEAD); // should be return 0xAD
}