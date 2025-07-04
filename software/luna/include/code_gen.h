#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "annotator.h"
#include "ast.h"
#include "instruction_builder.h"

struct CodeGenerator {
  struct ArenaAllocator *allocator;
  struct InstructionBuilder *instruction_builder;
  struct Annotator *annotator;
};

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct InstructionBuilder *instruction_builder,
                             struct Annotator *annotator);

void cg_visit_statements(struct CodeGenerator *code_generator,
                         struct ModuleStatementNode *stmt);

#endif
