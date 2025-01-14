void example_5(void) {
  short a = 100;
  short b = 0;

  short *src = &a;
  short *dst = &b;

  // copy an into b
  *dst = *src;
}
