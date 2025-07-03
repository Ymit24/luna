#include "code_gen.h"

struct CodeGenerator
code_generator_make(struct InstructionBuilder *instruction_builder) {
  return (struct CodeGenerator){
      .instruction_builder = instruction_builder,
  };
}
