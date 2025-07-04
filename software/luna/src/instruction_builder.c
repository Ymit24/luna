#include "instruction_builder.h"
#include "arena_allocator.h"
#include "ast.h"
#include "instructions.h"
#include "luna_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

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
      .next_label_number = 0,
  };

  // make preamble
  ib_push_fn(&instruction_builder);
  ib_push_label(&instruction_builder, string_make(".preamble"));
  // make statics
  ib_push_fn(&instruction_builder);
  ib_push_label(&instruction_builder, string_make(".statics"));

  assert(instruction_builder.head != NULL);
  assert(instruction_builder.head->next != NULL);
  assert(instruction_builder.current != NULL);

  return instruction_builder;
}

struct LunaString
ib_next_function_label(struct InstructionBuilder *instruction_builder) {
  char buff[64];
  uint16_t size = snprintf(buff, sizeof(buff), "__luna_anon_fn_%d",
                           instruction_builder->next_label_number++);

  char *str = arena_alloc(instruction_builder->allocator, size);

  memcpy(str, buff, size);

  return string_make(str);
}

struct LunaString ib_push_fn(struct InstructionBuilder *instruction_builder) {
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

  struct LunaString label = ib_next_function_label(instruction_builder);
  ib_push_label(instruction_builder, label);
  return label;
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

void ib_push_push_index(struct InstructionBuilder *instruction_builder,
                        enum MemorySegment memory_segment, uint16_t index) {
  ib_push_instruction(instruction_builder,
                      ast_promote(instruction_builder->allocator,
                                  &(struct Instruction){
                                      .type = IT_PUSH,
                                      .value.pushpoplea =
                                          (struct PushPopLeaInstruction){
                                              .memory_segment = memory_segment,
                                              .value.index = index,
                                              .is_index = true,
                                          }},
                                  sizeof(struct Instruction)),
                      NULL);
}

void ib_push_push_label(struct InstructionBuilder *instruction_builder,
                        struct LunaString label) {
  ib_push_instruction(
      instruction_builder,
      ast_promote(instruction_builder->allocator,
                  &(struct Instruction){.type = IT_PUSH,
                                        .value.pushpoplea =
                                            (struct PushPopLeaInstruction){
                                                .memory_segment = MS_CONST,
                                                .value.label = label,
                                                .is_index = false,
                                            }},
                  sizeof(struct Instruction)),
      NULL);
}

void ib_push_pop(struct InstructionBuilder *instruction_builder,
                 enum MemorySegment memory_segment, uint16_t index) {
  ib_push_instruction(instruction_builder,
                      ast_promote(instruction_builder->allocator,
                                  &(struct Instruction){
                                      .type = IT_POP,
                                      .value.pushpoplea =
                                          (struct PushPopLeaInstruction){
                                              .memory_segment = memory_segment,
                                              .value.index = index,
                                              .is_index = true,
                                          }},
                                  sizeof(struct Instruction)),
                      NULL);
}

void ib_push_lea(struct InstructionBuilder *instruction_builder,
                 enum MemorySegment memory_segment, uint16_t index) {
  ib_push_instruction(instruction_builder,
                      ast_promote(instruction_builder->allocator,
                                  &(struct Instruction){
                                      .type = IT_LEA,
                                      .value.pushpoplea =
                                          (struct PushPopLeaInstruction){
                                              .memory_segment = memory_segment,
                                              .value.index = index,
                                              .is_index = true,
                                          }},
                                  sizeof(struct Instruction)),
                      NULL);
}

void ib_push_label(struct InstructionBuilder *instruction_builder,
                   struct LunaString label) {
  ib_push_instruction(
      instruction_builder,
      ast_promote(instruction_builder->allocator,
                  &(struct Instruction){.type = IT_LABEL, .value.label = label},
                  sizeof(struct Instruction)),
      NULL);
}

void ib_push_argless(struct InstructionBuilder *instruction_builder,
                     enum InstructionType type) {
  ib_push_instruction(instruction_builder,
                      ast_promote(instruction_builder->allocator,
                                  &(struct Instruction){.type = type},
                                  sizeof(struct Instruction)),
                      NULL);
}
