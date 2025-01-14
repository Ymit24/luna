#include <string.h>
void example_5(void) {
  short a = 100;
  short b = 0;

  short *src = &a;
  short *dst = &b;

  // copy an into b
  *dst = *src;
}

void memcpy2(void *src, void *dst, short len) {
  // convert to short * so we copy 16 bits at a time, the memory size.
  const short *s = src;
  short *d = dst;

  for (short i = 0; i < len; i++) {
    d[i] = s[i];
  }
}

struct two {
  short a;
  short b;
};

void assert(char);

void ex(void) {
  struct two x = {.a = 1, .b = 2};
  struct two y = {0};

  memcpy2(&x, &y, sizeof(struct two));

  short eq = y.a == 1;

  assert(y.a == 1);
  assert(y.a == 2);
}
