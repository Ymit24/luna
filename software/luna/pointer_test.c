#include <stdio.h>
#include <stdint.h>

extern void _4(void *p);

int main(void) {
  int32_t x = 10;
  printf("before: x=%d\n", x);
  _4(&x);
  printf("after:  x=%d\n", x);
  return 0;
}
