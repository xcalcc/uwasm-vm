#ifdef TEST
#include <stdlib.h>
#include <stdio.h>
#endif

// recursion where local value must be preserved in between a recursive call

int self_recurse(int x, int count)
{
  int local_int = 20;

  if (count == 0)
    return 0;

  count--;
#ifdef TEST
  printf("x = %d count %d\n",x, count);
#endif

  if (x == 18) {
    local_int = x+local_int;
    x--;
    x += self_recurse(x, count);
  }

  local_int -= 2;
  if ((x & 0x1) == 1) {
    local_int -= self_recurse(local_int, count);
    
  }
#ifdef TEST
  printf("local_int %d\n",local_int);
#endif
  return 0;
  
}


int main(void)
{
 
  int i = self_recurse(7, 5);
}
