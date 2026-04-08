#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "test.h"

int exec_test(void (*test)(void)) {
  int stat;

  if (fork() == 0) {
    int dev_null = open("/dev/null", O_WRONLY);
    if (dev_null != -1) {
      dup2(dev_null, STDERR_FILENO);
      close(dev_null);
    }
    test();
    exit(0);
    return 0;
  } else {
    wait(&stat);
  }

  return stat == 0;
}

static int PASS_COUNT = 0;
static int TEST_COUNT = 0;

void run_test(char *name, void (*test)(void)) {
  TEST_COUNT++;
  if (exec_test(test)) {
    printf("[ PASSED ] %s\n", name);
    PASS_COUNT++;
  } else {
    printf("[ FAILED ] %s\n", name);
  }
}

void print_results(void) {
  printf("\n\n%d passed, %d failed, %d total\n", PASS_COUNT, TEST_COUNT-PASS_COUNT, TEST_COUNT);
}
