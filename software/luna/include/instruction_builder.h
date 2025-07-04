#ifndef INSTRUCTION_BUILDER_H
#define INSTRUCTION_BUILDER_H

#include "arena_allocator.h"
#include "instructions.h"
#include "luna_string.h"
#include <stdint.h>

struct InstructionGroup {
  struct Instruction *head;
  struct Instruction *tail;
  struct InstructionGroup *prev;
  struct InstructionGroup *next;
};

struct InstructionBuilder {
  struct ArenaAllocator *allocator;
  struct InstructionGroup *head;
  struct InstructionGroup *current;
};

struct InstructionBuilder
instruction_builder_make(struct ArenaAllocator *allocator);

void ib_push_fn(struct InstructionBuilder *instruction_builder);
void ib_pop_fn(struct InstructionBuilder *instruction_builder);
void ib_push_instruction(struct InstructionBuilder *instruction_builder,
                         struct Instruction *instr,
                         struct InstructionGroup *group);

void ib_push_push(struct InstructionBuilder *instruction_builder,
                  enum MemorySegment memory_segment, uint16_t index);
void ib_push_pop(struct InstructionBuilder *instruction_builder,
                 enum MemorySegment memory_segment, uint16_t index);
void ib_push_lea(struct InstructionBuilder *instruction_builder,
                 enum MemorySegment memory_segment, uint16_t index);
void ib_push_label(struct InstructionBuilder *instruction_builder,
                 struct LunaString label);
void ib_push_argless(struct InstructionBuilder *instruction_builder,
                 enum InstructionType type);

#endif
