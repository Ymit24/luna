#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "instruction_builder.h"

struct CodeGenerator {
  struct InstructionBuilder *instruction_builder;
};

struct CodeGenerator code_generator_make(struct InstructionBuilder* instruction_builder);

#endif
