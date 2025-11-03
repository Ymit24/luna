#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "lexer.h"
#include "llvm-backend/code_generator.h"
#include "luna_string.h"
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

int main(int argc, char **argv) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX * 4];

  struct ArenaAllocator allocator = arena_make(&arena, UINT16_MAX * 4);

  if (argc < 3) {
    fprintf(stderr, "usage: %s <source.luna> <output.ll>\n", argv[0]);
    return 1;
  }

  struct LunaString src = read_file(argv[1], &allocator);
  struct Lexer lexer = lexer_make(&allocator, src);

  struct Token toks[1024];
  uint16_t tok_index = 0;

  puts("tokens:");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    if (false) {
      printf("\t%d\n", toks[tok_index - 1].type);
    }
  }
  puts("");

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(&allocator, toks, tok_index);

  struct ModuleStatementNode *stmt = parse_module_statements(&parser);

  struct ModuleNode file_module = (struct ModuleNode){
      .statements = stmt,
  };

  struct ExpressionNode file_module_definition = (struct ExpressionNode){
      .type = EXPR_MOD_DEF, .node.module_definition = &file_module

  };

  struct DeclarationStatementNode root_decl = (struct DeclarationStatementNode){
      .symbol = string_make("ROOT"),
      .data_type = NULL,
      .has_type = false,
      .is_const = true,
      .expression = &file_module_definition,
  };

  struct ModuleStatementNode new_root = (struct ModuleStatementNode){
      .type = MOD_STMT_CONST,
      .node.decl = &root_decl,
      .next = NULL,
  };

  puts("done parsing");

  struct Annotator annotator = annotator_make(&allocator);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  annotator_initialize_primitives(&annotator);
  annotator_visit_module_statements(&annotator, &new_root);

  print_symbol_table(string_make("Root"), &annotator.root_symbol_table);

  struct CodeGenerator code_generator = cg_make(&allocator, &annotator);

  puts("Start code gen");
  // LLVMValueRef global_module_initializer =
  cg_visit_module_statements(&code_generator, &new_root, true, string_make("core"));
  // cg_make_entrypoint(&code_generator, global_module_initializer);
  puts("Done code gen");

  LLVMDumpModule(code_generator.module);

  char *error = NULL;
  if (LLVMPrintModuleToFile(code_generator.module, argv[2], &error) != 0) {
    fprintf(stderr, "Failed to write module: %s", error);
    LLVMDisposeMessage(error);
  }

  printf("Arena Allocator used %ld/%ld (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
