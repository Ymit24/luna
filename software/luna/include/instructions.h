#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "luna_string.h"
#include <stdint.h>

enum InstructionType {
  IT_PUSH,
  IT_POP,
  IT_LABEL,
  IT_ADD,
  IT_SUB,
};

enum MemorySegment { MS_LOCAL };

struct PushInstruction {
  enum MemorySegment memory_segment;
  uint16_t index;
};

struct PopInstruction {
  enum MemorySegment memory_segment;
  uint16_t index;
};

struct Instruction {
  enum InstructionType type;
  union {
    struct PushInstruction push;
    struct PopInstruction pop;
    struct LunaString label;
  } value;
  struct Instruction *next;
};

#endif
