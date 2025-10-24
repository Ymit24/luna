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

int main(void) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX];

  struct ArenaAllocator allocator = arena_make(&arena, UINT16_MAX);

  struct Lexer lexer = lexer_make(&allocator, string_make("let a = 5 - (2 + 1);"
                                                          "let x = 3;"
                                                          "let g: int = 5;"
                                                          "let other = 100;"
                                                          "let main = fn() {"
                                                          "  const thing = 10;"
                                                          "  const inner = fn() {"
                                                          "    const otheragain = 20;"
                                                          "  };"
                                                          "  const next = 9;"
                                                          "};"));

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

  puts("done parsing");

  struct Annotator annotator = annotator_make(&allocator);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  annotator_initialize_primitives(&annotator);
  annotator_visit_module_statements(&annotator, stmt);

  struct CodeGenerator code_generator = cg_make(&allocator, &annotator);

  puts("Start code gen");
  cg_visit_module_statements(&code_generator, stmt);
  puts("Done code gen");

  LLVMDumpModule(code_generator.module);

  char *error = NULL;
  if (LLVMPrintModuleToFile(code_generator.module, "compiled_luna.ll",
                            &error) != 0) {
    fprintf(stderr, "Failed to write module: %s", error);
    LLVMDisposeMessage(error);
  }

  printf("Arena Allocator used %d/%d (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
