#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef VERBOSE_LOG

#define DEBUG_LOG(...) do {               \
    fprintf(stderr, "[DEBUG] ");          \
    fprintf(stderr, __VA_ARGS__);         \
    fprintf(stderr, "\n");                \
} while (0)

#else

#define DEBUG_LOG(...) do { if (0) { fprintf(stderr, __VA_ARGS__); } } while (0)

#endif

#endif
