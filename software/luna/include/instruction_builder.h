#ifndef INSTRUCTION_BUILDER_H
#define INSTRUCTION_BUILDER_H

#include "luna_string.h"
#include <stdio.h>
struct InstructionBuilder {
  FILE *output_file;
};

struct InstructionBuilder instruction_builder_make(FILE *output_file);

void instr_write_line(struct InstructionBuilder *instruction_builder,
                      struct LunaString line);

extern uint16_t IB_SP_ADDR, IB_BSP_ADDR, IB_NP_ADDR;

// push $1 $2
void instr_push_push(struct InstructionBuilder *instruction_builder,
                     struct LunaString memory_segment, uint16_t location);
// pop $1 $2
void instr_push_pop(struct InstructionBuilder *instruction_builder,
                    struct LunaString memory_segment, uint16_t location);
// // add
// void instr_push_add(struct InstructionBuilder *instruction_builder);
// // sub
// void instr_push_sub(struct InstructionBuilder *instruction_builder);
// // $main
// void instr_push_label(struct InstructionBuilder *instruction_builder,
//                       struct LunaString label);
// // ret
// void instr_push_return(struct InstructionBuilder *instruction_builder);
// // call $1
// void instr_push_call(struct InstructionBuilder *instruction_builder,
//                      struct LunaString label);

#endif
