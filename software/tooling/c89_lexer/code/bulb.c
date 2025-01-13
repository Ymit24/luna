void example_0(void) { short foo = 5; }

void example_1(void) {
  short foo = 5;
  short bar = foo;
}

void example_2(void) {
  short foo = 5;
  short bar = foo + 0xFF;
}

void example_3(void) {
  short foo = 1;
  short bar = 0;

  if (foo == 1) {
    bar = 1;
  }
}

void example_4(short n) {
  while (n > 0) {
    n--;
  }
}

typedef struct {
  short foo;
  char name[4];
  char *name2;
} __luna_statics;

__luna_statics LUNA_STATICS = (__luna_statics){
    .foo = 100,
    .name = "foo",
    .name2 = "o",
};

void test(void) {
  static short foo = 100; // -> LUNA_STATICS.foo
  static char name[] = "foo";
  static char *name2 = "o";
}
