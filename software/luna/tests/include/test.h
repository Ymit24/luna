#ifndef TEST_H
#define TEST_H

int exec_test(void (*test)(void));
void run_test(char *name, void (*test)(void));
void print_results(void);

#endif
