#include "instruction_builder.h"
#include "arena_allocator.h"
#include "ast.h"
#include <assert.h>

struct InstructionGroup *
get_preamble_group(struct InstructionBuilder *instruction_builder) {
  return instruction_builder->head;
}

struct InstructionGroup *
get_static_group(struct InstructionBuilder *instruction_builder) {
  return instruction_builder->head->next;
}

struct InstructionBuilder
instruction_builder_make(struct ArenaAllocator *allocator) {
  struct InstructionBuilder instruction_builder = (struct InstructionBuilder){
      .allocator = allocator,
      .head = NULL,
      .current = NULL,
  };

  // make preamble
  ib_push_fn(&instruction_builder);
  // make statics
  ib_push_fn(&instruction_builder);

  assert(instruction_builder.head != NULL);
  assert(instruction_builder.head->next != NULL);
  assert(instruction_builder.current != NULL);

  return instruction_builder;
}

void ib_push_fn(struct InstructionBuilder *instruction_builder) {
  struct InstructionGroup *group = ast_promote(
      instruction_builder->allocator,
      &(struct InstructionGroup){.prev = instruction_builder->current,
                                 .next = NULL,
                                 .head = NULL,
                                 .tail = NULL},
      sizeof(struct InstructionGroup));

  if (instruction_builder->current != NULL) {
    instruction_builder->current->next = group;
  }

  if (instruction_builder->head == NULL) {
    instruction_builder->head = group;
  }

  instruction_builder->current = group;
}

// NOTE: This does not remove function, just finishes the generation, for nested
// functions
void ib_pop_fn(struct InstructionBuilder *instruction_builder) {
  assert(instruction_builder->current != NULL);

  instruction_builder->current = instruction_builder->current->prev;
}

void ib_push_instruction(struct InstructionBuilder *instruction_builder,
                         struct Instruction *instr,
                         struct InstructionGroup *group) {
  assert(instruction_builder->current != NULL);

  if (group == NULL) {
    group = instruction_builder->current;
  }

  assert(group != NULL);

  if (instruction_builder->current->tail == NULL) {
    assert(instruction_builder->current->head == NULL);
    instruction_builder->current->tail = instr;
    instruction_builder->current->head = instr;
  } else {
    assert(instruction_builder->current->head != NULL);
    instruction_builder->current->tail->next = instr;
    instruction_builder->current->tail = instr;
  }
}
