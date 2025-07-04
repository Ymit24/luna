#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "luna_string.h"
#include <stdint.h>

enum InstructionType {
  IT_PUSH,
  IT_POP,
  IT_LOAD,
  IT_LABEL,
  IT_ADD,
  IT_SUB,
};

enum MemorySegment { MS_LOCAL, MS_CONST };

struct PushPopInstruction {
  enum MemorySegment memory_segment;
  uint16_t index;
};

struct Instruction {
  enum InstructionType type;
  union {
    struct PushPopInstruction pushpop;
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
 * push const %bsp
 * load
 * push const 0
 * add
 * pop local 1
 *
 *
 * push local 0
 * push local 1
 * load
 * add
 * pop local 2
 *
 *
 */
