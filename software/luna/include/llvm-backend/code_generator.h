#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "annotator.h"
#include "arena_allocator.h"
#include "llvm-c/Types.h"
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>

struct CodeGenerator {
  struct ArenaAllocator *allocator;
  struct Annotator *annotator;
  struct SymbolTable *current_symbol_table;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
  LLVMBasicBlockRef current_block;
  LLVMTargetDataRef target_data;
};

struct FindFieldDefinitionResult {
  struct StructFieldDefinitionNode *field_definition;
  size_t index;
};

struct CodeGenerator cg_make(struct ArenaAllocator *allocator,
                             struct Annotator *annotator);

LLVMValueRef cg_visit_module_statements(struct CodeGenerator *code_generator,
                                        struct ModuleStatementNode *stmt,
                                        bool is_root);

#endif
