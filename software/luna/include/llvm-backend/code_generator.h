#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "annotator.h"
#include "arena_allocator.h"
#include "llvm-c/Types.h"

struct CodeGenerator {
  struct ArenaAllocator *allocator;
  struct Annotator *annotator;
  struct SymbolTable *current_symbol_table;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
  LLVMBasicBlockRef current_block;
};

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct Annotator *annotator);

LLVMValueRef cg_visit_module_statements(struct CodeGenerator *code_generator,
                                        struct ModuleStatementNode *stmt,
                                        bool is_root);

#endif
