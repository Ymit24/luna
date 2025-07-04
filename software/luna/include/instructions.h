#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "luna_string.h"
#include <stdbool.h>
#include <stdint.h>

enum InstructionType {
  IT_PUSH,
  IT_POP,
  IT_LOAD,
  IT_STORE,
  IT_LEA,
  IT_LABEL,
  IT_ADD,
  IT_SUB,
};

enum MemorySegment { MS_LOCAL, MS_CONST };

struct PushPopLeaInstruction {
  enum MemorySegment memory_segment;
  union {
    uint16_t index;
    struct LunaString label;
  } value;
  bool is_index;
};

struct Instruction {
  enum InstructionType type;
  union {
    struct PushPopLeaInstruction pushpoplea;
    struct LunaString label;
  } value;
  struct Instruction *next;
};

#endif

/*
 * let a = 10 // a @ bsp+0
 * let b = 5  // b @ bsp+1
 * b += a
 *
 * -----
 *
 * push const 10
 * pop local 0
 *
 * push const 5
 * pop local 1
 *
 * push local 0
 * push local 1
 * add
 * pop local 1
 *
 * -----
 *
 * let a = 10 // a @ bsp+0
 * let b = &a;// b @ bsp+1
 * let c = a + *b; // c @ bsp+2
 *
 * -----
 *
 * push const 10
 * pop local 0
 *
 * lea local 0
 * pop local 1
 *
 * push local 0
 * push local 1
 * load
 * add
 * pop local 2
 *
 */
