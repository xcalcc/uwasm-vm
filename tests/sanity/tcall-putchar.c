extern int putchar(int);

int main() {
  char ch;
  for (ch = 'A'; ch < 'F'; ch++) {
    putchar(ch);
  }

  putchar('\n');
  return 0;
}
