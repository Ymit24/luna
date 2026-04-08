#include <stdio.h>
#include <assert.h>

#include "test.h"

int sum(int a, int b) { return a + b; }

void test_sum_good(void) { assert(sum(2, 4) == 6); }

void test_sum_bad(void) { assert(sum(2, 4) == 7); }

int main(void) {
  run_test("Test sum good", &test_sum_good);
  run_test("Test sum bad", &test_sum_bad);

  print_results();
  return 0;
}
