#include <llvm-c/Types.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "lexer.h"
#include "llvm-backend/code_generator.h"
#include "luna_string.h"
#include "module_symbol_table_builder.h"
#include "parser.h"
#include "token.h"
#include "llvm-c/Core.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "lexer.h"
#include "llvm-backend/code_generator.h"
#include "luna_string.h"
#include "module_symbol_table_builder.h"
#include "parser.h"
#include "token.h"
#include "llvm-c/Core.h"

struct LunaString read_file(const char *path, struct ArenaAllocator *alloc) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    fprintf(stderr, "could not open file: %s\n", path);
    assert(0);
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = arena_alloc(alloc, (uint16_t)size + 1);
  if (!buffer) {
    fprintf(stderr, "not enough arena space for source\n");
    assert(0);
  }
  size_t n = fread(buffer, 1, (size_t)size, file);
  fclose(file);
  buffer[n] = 0;
  return (struct LunaString){.data = buffer, .length = (uint16_t)n};
}

struct ModuleStatementNode *parse_source_file(struct ArenaAllocator *allocator,
                                              char *source_file) {

  struct LunaString src = read_file(source_file, allocator);
  struct Lexer lexer = lexer_make(allocator, src);

  struct Token toks[4096];
  uint16_t tok_index = 0;

  while (lexer_next(&lexer, &toks[tok_index++])) {
  }

  struct Parser parser = parser_make(allocator, toks, tok_index);

  struct ModuleStatementNode *stmt = parse_module_statements(&parser);

  return stmt;
}

struct ModuleStatementNode *wrap_module(struct ArenaAllocator *allocator,
                                        struct ModuleStatementNode *old_root,
                                        struct LunaString file_name) {

  struct ModuleNode *file_module = ast_promote(allocator,
                                               &(struct ModuleNode){
                                                   .statements = old_root,
                                               },
                                               sizeof(struct ModuleNode));

  struct ExpressionNode *file_module_definition = ast_promote_expression_node(
      allocator,
      (struct ExpressionNode){.type = EXPR_MOD_DEF,
                              .node.module_definition = file_module});

  struct DeclarationStatementNode *root_decl =
      ast_promote(allocator,
                  &(struct DeclarationStatementNode){
                      .symbol = file_name,
                      .data_type = NULL,
                      .has_type = false,
                      .is_const = true,
                      .expression = file_module_definition,
                  },
                  sizeof(struct DeclarationStatementNode));

  struct ModuleStatementNode *new_root =
      ast_promote(allocator,
                  &(struct ModuleStatementNode){
                      .type = MOD_STMT_CONST,
                      .node.decl = root_decl,
                      .next = NULL,
                  },
                  sizeof(struct ModuleStatementNode));

  return new_root;
}

struct LunaString get_module_name_from_file(struct ArenaAllocator *allocator,
                                            char *file_path) {
  const char *last_slash = strrchr(file_path, '/');
  const char *result = last_slash ? last_slash + 1 : file_path;
  const char *last_dot = strrchr(result, '.');

  size_t size;
  if (last_dot && last_dot > result)
    size = (size_t)(last_dot - result);
  else
    size = strlen(result);

  char *new_str = arena_alloc(allocator, size + 1);
  memcpy(new_str, result, size);
  new_str[size] = '\0';

  return string_make(new_str);
}

struct ModuleStatementNode *parse_files(struct ArenaAllocator *allocator,
                                        char **source_file_paths,
                                        size_t source_file_count) {
  struct ModuleStatementNode *new_root = NULL;
  struct ModuleStatementNode *current = NULL;

  for (int i = 1; i < (int)source_file_count; i++) {
    struct ModuleStatementNode *source_root =
        parse_source_file(allocator, source_file_paths[i]);

    if (current != NULL) {
      current->next = wrap_module(
          allocator, source_root,
          get_module_name_from_file(allocator, source_file_paths[i]));
      current = current->next;
    } else {
      new_root = wrap_module(
          allocator, source_root,
          get_module_name_from_file(allocator, source_file_paths[i]));
      current = new_root;
    }
  }
  new_root = wrap_module(allocator, new_root, string_make("src"));

  return new_root;
}

void code_gen(struct CodeGenerator *code_generator,
              struct ModuleStatementNode *root) {
  cg_prepare_module(code_generator, root);
  LLVMValueRef global_module_initializer = cg_visit_module_statements(
      code_generator, root, true, string_make("core"));
  cg_make_entrypoint(code_generator, global_module_initializer);
}

void emit_module_to_file(LLVMModuleRef module, char *output_filepath) {
  char *error = NULL;
  if (LLVMPrintModuleToFile(module, output_filepath, &error) != 0) {
    fprintf(stderr, "Failed to write module: %s", error);
    LLVMDisposeMessage(error);
  }
}

void annotate(struct Annotator *annotator, struct ModuleStatementNode *root) {
  mstb_visit_module(annotator, root);
  insert_symbol_entry(annotator, (struct SymbolTableEntry){
                                     .symbol = string_make("src"),
                                     .type = root->node.decl->data_type,
                                     .llvm_value = NULL,
                                     .next = NULL,
                                     .symbol_location = SL_MODULE,
                                     .index = 0,
                                 });

  mstb_infer_types(
      annotator,
      &root->node.decl->expression->node.module_definition->symbol_table);
  annotator->current_symbol_table = &annotator->root_symbol_table;

  annotator_initialize_primitives(annotator);
  annotator_visit_module_statements(annotator, root);
}

void compile(struct ArenaAllocator *allocator, char **source_file_paths,
             size_t source_file_count, char *output_filepath) {
  struct ModuleStatementNode *root =
      parse_files(allocator, source_file_paths, source_file_count);

  struct Annotator annotator = annotator_make(allocator);
  annotate(&annotator, root);

  struct CodeGenerator code_generator = cg_make(allocator, &annotator);
  code_gen(&code_generator, root);

  emit_module_to_file(code_generator.module, output_filepath);
}
