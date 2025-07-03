#ifndef INSTRUCTION_BUILDER_H
#define INSTRUCTION_BUILDER_H

#include "arena_allocator.h"
#include "instructions.h"

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

#endif
