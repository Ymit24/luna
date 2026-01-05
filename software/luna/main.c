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

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }
}

static void print_tree_connector(int indent, bool is_last) {
  print_indent(indent);
  printf("%s ", is_last ? "└─" : "├─");
}

static void print_span(struct SourceLocation loc) {
  printf("[file=%.*s, span=%d-%d]", (int)loc.filename.length,
         loc.filename.data, loc.start, loc.end);
}

static void print_expression_node(struct ExpressionNode *node, int indent);

static void print_binary_node(struct BinaryExpressionNode *node, int indent) {
  print_tree_connector(indent, false);
  printf("left: ");
  print_expression_node(node->left, 0);
  printf("\n");

  print_tree_connector(indent, false);
  printf("right: ");
  print_expression_node(node->right, 0);
  printf("\n");
}

static void print_expression_node(struct ExpressionNode *node, int indent) {
  (void)indent;
  if (!node) {
    printf("(null)");
    return;
  }

  switch (node->type) {
  case EXPR_BINARY: {
    printf("ExpressionNode (EXPR_BINARY) ");
    print_span(node->location);
    printf("\n");
    print_binary_node(node->node.binary, indent + 1);
    break;
  }
  case EXPR_INTEGER_LITERAL:
    printf("ExpressionNode (EXPR_INTEGER_LITERAL) ");
    print_span(node->location);
    printf(" value=%lld", (long long)node->node.integer->value);
    break;
  case EXPR_SYMBOL_LITERAL:
    printf("ExpressionNode (EXPR_SYMBOL_LITERAL) ");
    print_span(node->location);
    printf(" symbol=%.*s", (int)node->node.scoped_symbol->symbol->value.length,
           node->node.scoped_symbol->symbol->value.data);
    break;
  case EXPR_STRING_LITERAL:
    printf("ExpressionNode (EXPR_STRING_LITERAL) ");
    print_span(node->location);
    printf(" value=%.*s", (int)node->node.string->value.length,
           node->node.string->value.data);
    break;
  case EXPR_FN_DEF:
    printf("ExpressionNode (EXPR_FN_DEF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("function_definition\n");
    break;
  case EXPR_FN_CALL:
    printf("ExpressionNode (EXPR_FN_CALL) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_REF:
    printf("ExpressionNode (EXPR_REF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.ref, 0);
    printf("\n");
    break;
  case EXPR_DEREF:
    printf("ExpressionNode (EXPR_DEREF) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.deref, 0);
    printf("\n");
    break;
  case EXPR_NOT:
    printf("ExpressionNode (EXPR_NOT) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.not, 0);
    printf("\n");
    break;
  case EXPR_STRUCT_DEF:
    printf("ExpressionNode (EXPR_STRUCT_DEF) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_STRUCT_INIT:
    printf("ExpressionNode (EXPR_STRUCT_INIT) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_FIELD_ACCESS:
    printf("ExpressionNode (EXPR_FIELD_ACCESS) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, false);
    printf("expression: ");
    print_expression_node(node->node.struct_field_access->expression, 0);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("field=%.*s\n",
           (int)node->node.struct_field_access->symbol.length,
           node->node.struct_field_access->symbol.data);
    break;
  case EXPR_CAST:
    printf("ExpressionNode (EXPR_CAST) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("inner: ");
    print_expression_node(node->node.cast->expr, 0);
    printf("\n");
    break;
  case EXPR_ARRAY_INITIALIZER:
    printf("ExpressionNode (EXPR_ARRAY_INITIALIZER) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_VALUESIZE:
    printf("ExpressionNode (EXPR_VALUESIZE) ");
    print_span(node->location);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("expr: ");
    print_expression_node(node->node.valuesize, 0);
    printf("\n");
    break;
  case EXPR_TYPESIZE:
    printf("ExpressionNode (EXPR_TYPESIZE) ");
    print_span(node->location);
    printf("\n");
    break;
  case EXPR_MOD_DEF:
    printf("ExpressionNode (EXPR_MOD_DEF) ");
    print_span(node->location);
    printf("\n");
    break;
  default:
    printf("ExpressionNode (UNKNOWN) ");
    print_span(node->location);
    break;
  }
}

static void print_function_statement_node(struct FunctionStatementNode *stmt,
                                       int indent, bool is_last);

static void print_function_statement_list(struct FunctionStatementNode *head,
                                        int indent) {
  struct FunctionStatementNode *curr = head;
  while (curr) {
    struct FunctionStatementNode *next = curr->next;
    print_function_statement_node(curr, indent, next == NULL);
    curr = next;
  }
}

static void print_function_statement_node(struct FunctionStatementNode *stmt,
                                       int indent, bool is_last) {
  if (!stmt) {
    print_tree_connector(indent, is_last);
    printf("(null)\n");
    return;
  }

  const char *type_str;
  switch (stmt->type) {
  case FN_STMT_LET:
    type_str = "FN_STMT_LET";
    break;
  case FN_STMT_CONST:
    type_str = "FN_STMT_CONST";
    break;
  case FN_STMT_ASSIGN:
    type_str = "FN_STMT_ASSIGN";
    break;
  case FN_STMT_RETURN:
    type_str = "FN_STMT_RETURN";
    break;
  case FN_STMT_FN_CALL:
    type_str = "FN_STMT_FN_CALL";
    break;
  case FN_STMT_IF:
    type_str = "FN_STMT_IF";
    break;
  case FN_STMT_WHILE:
    type_str = "FN_STMT_WHILE";
    break;
  default:
    type_str = "UNKNOWN";
    break;
  }

  print_tree_connector(indent, is_last);
  printf("FunctionStatementNode (%s) ", type_str);
  print_span(stmt->location);
  printf("\n");

  switch (stmt->type) {
  case FN_STMT_LET:
  case FN_STMT_CONST:
    print_tree_connector(indent + 1, false);
    printf("symbol: %.*s\n", (int)stmt->node.decl->symbol.length,
           stmt->node.decl->symbol.data);
    if (stmt->node.decl->has_type) {
      print_tree_connector(indent + 1, false);
      printf("has_type: true\n");
    } else {
      print_tree_connector(indent + 1, false);
      printf("has_type: false\n");
    }
    print_tree_connector(indent + 1, false);
    printf("is_const: %s\n", stmt->type == FN_STMT_CONST ? "true" : "false");
    print_tree_connector(indent + 1, true);
    printf("expression: ");
    print_expression_node(stmt->node.decl->expression, 0);
    printf("\n");
    break;

  case FN_STMT_RETURN:
    if (stmt->node.ret->expression) {
      print_tree_connector(indent + 1, true);
      printf("expression: ");
      print_expression_node(stmt->node.ret->expression, 0);
      printf("\n");
    } else {
      print_tree_connector(indent + 1, true);
      printf("expression: (void)\n");
    }
    break;

  case FN_STMT_FN_CALL:
    print_tree_connector(indent + 1, true);
    printf("function_call\n");
    break;

  case FN_STMT_IF: {
    print_tree_connector(indent + 1, false);
    printf("condition: ");
    print_expression_node(stmt->node.if_stmt->condition, 0);
    printf("\n");
    print_tree_connector(indent + 1, false);
    printf("body:\n");
    print_function_statement_list(stmt->node.if_stmt->body, indent + 2);
    if (stmt->node.if_stmt->next) {
      print_tree_connector(indent + 1, true);
      printf("else:\n");
      struct IfStatementNode *curr = stmt->node.if_stmt->next;
      while (curr) {
        if (curr->condition) {
          print_tree_connector(indent + 2, curr->next == NULL);
          printf("condition: ");
          print_expression_node(curr->condition, 0);
          printf("\n");
          print_tree_connector(indent + 2, curr->next == NULL);
          printf("body:\n");
          print_function_statement_list(curr->body, indent + 3);
        } else {
          print_tree_connector(indent + 2, true);
          printf("body:\n");
          print_function_statement_list(curr->body, indent + 3);
        }
        curr = curr->next;
      }
    }
    break;
  }

  case FN_STMT_WHILE:
    print_tree_connector(indent + 1, false);
    printf("condition: ");
    print_expression_node(stmt->node.while_stmt->condition, 0);
    printf("\n");
    print_tree_connector(indent + 1, true);
    printf("body:\n");
    print_function_statement_list(stmt->node.while_stmt->body, indent + 2);
    break;

  default:
    break;
  }
}

static void print_module_statement_node(struct ModuleStatementNode *stmt,
                                      int indent, bool is_last) {
  if (!stmt) {
    print_tree_connector(indent, is_last);
    printf("(null)\n");
    return;
  }

  const char *type_str;
  switch (stmt->type) {
  case MOD_STMT_LET:
    type_str = "MOD_STMT_LET";
    break;
  case MOD_STMT_CONST:
    type_str = "MOD_STMT_CONST";
    break;
  default:
    type_str = "UNKNOWN";
    break;
  }

  print_tree_connector(indent, is_last);
  printf("ModuleStatementNode (%s) ", type_str);
  print_span(stmt->location);
  printf("\n");

  print_tree_connector(indent + 1, false);
  printf("symbol: %.*s\n", (int)stmt->node.decl->symbol.length,
         stmt->node.decl->symbol.data);

  if (stmt->node.decl->has_type) {
    print_tree_connector(indent + 1, false);
    printf("has_type: true\n");
  } else {
    print_tree_connector(indent + 1, false);
    printf("has_type: false\n");
  }

  print_tree_connector(indent + 1, false);
  printf("is_const: %s\n", stmt->type == MOD_STMT_CONST ? "true" : "false");

  print_tree_connector(indent + 1, true);
  printf("expression: ");
  print_expression_node(stmt->node.decl->expression, 0);
  printf("\n");
}

static void print_module_statement_list(struct ModuleStatementNode *head,
                                      int indent) {
  (void)head;
  (void)indent;
  struct ModuleStatementNode *curr = head;
  while (curr) {
    struct ModuleStatementNode *next = curr->next;
    print_module_statement_node(curr, indent, next == NULL);
    curr = next;
  }
}

const char *VERSION = "0.1.3-dev";

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
  struct LunaString filename = string_make(source_file);
  struct Lexer lexer = lexer_make(allocator, src, filename);

  struct Token toks[4096];
  uint16_t tok_index = 0;

  puts("tokens:");
  while (lexer_next(&lexer, &toks[tok_index++])) {
    struct Token t = toks[tok_index - 1];
    uint16_t span_len = t.location.end - t.location.start;
    printf("Token %d: file=%.*s, span=%d-%d (len=%d), text=\"%.*s\"\n",
           t.type,
           (int)t.location.filename.length, t.location.filename.data,
           t.location.start, t.location.end, span_len,
           (int)span_len, src.data + t.location.start);
  }
  puts("");

  printf("Found a total of %d tokens.\n", tok_index - 1);

  struct Parser parser = parser_make(allocator, toks, tok_index);

  struct ModuleStatementNode *stmt = parse_module_statements(&parser);

  puts("done parsing");
  puts("\n\nAST Structure:");
  print_module_statement_list(stmt, 0);
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

int main(int argc, char **argv) {
  puts("Luna Compiler");

  uint8_t arena[UINT16_MAX * 16];

  struct ArenaAllocator allocator = arena_make(&arena, sizeof(arena));

  if (argc < 2) {
    fprintf(stderr, "usage: %s <source.luna> ...\n", argv[0]);
    return 1;
  }

  if (strcmp(argv[1], "-v") == 0) {
    printf("version: %s\n", VERSION);
    return 0;
  }

  struct ModuleStatementNode *new_root = NULL;
  struct ModuleStatementNode *current = NULL;
  for (int i = 1; i < (int)argc; i++) {
    printf("parsing file (%d)->(%s)\n", i, argv[i]);
    struct ModuleStatementNode *source_root =
        parse_source_file(&allocator, argv[i]);

    if (current != NULL) {
      current->next =
          wrap_module(&allocator, source_root,
                      get_module_name_from_file(&allocator, argv[i]));
      current = current->next;
    } else {
      new_root = wrap_module(&allocator, source_root,
                             get_module_name_from_file(&allocator, argv[i]));
      current = new_root;
    }
  }
  new_root = wrap_module(&allocator, new_root, string_make("src"));

  puts("Debug mode - skipping annotate/codegen");
  return 0;

  struct Annotator annotator = annotator_make(&allocator);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  puts("\n\n\n\n\n\n");
  puts("----------------------------------------------------------");
  puts("Starting mstb visitation.");
  puts("\n\n");

  mstb_visit_module(&annotator, new_root);

  insert_symbol_entry(&annotator, (struct SymbolTableEntry){
                                      .symbol = string_make("src"),
                                      .type = new_root->node.decl->data_type,
                                      .llvm_value = NULL,
                                      // .llvm_structure_type = NULL,
                                      .next = NULL,
                                      .symbol_location = SL_MODULE,
                                      .index = 0,
                                  });

  puts("Finished mstb.");

  mstb_infer_types(
      &annotator,
      &new_root->node.decl->expression->node.module_definition->symbol_table);

  puts("Finished infer types");

  print_symbol_table(string_make("Root1"), &annotator.root_symbol_table);
  annotator.current_symbol_table = &annotator.root_symbol_table;

  // annotator.root_symbol_table =
  //     new_root->node.decl->expression->node.module_definition->symbol_table;

  puts("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  puts("\n\n\n\n\n\n");

  // assert(0);

  annotator_initialize_primitives(&annotator);
  puts("About to visit module statements.");
  annotator_visit_module_statements(&annotator, new_root);
  puts("Done visit module statements.");

  print_symbol_table(string_make("Root2"), &annotator.root_symbol_table);

  // assert(0);

  struct CodeGenerator code_generator = cg_make(&allocator, &annotator);

  puts("Start code gen");
  cg_prepare_module(&code_generator, new_root);
  puts("Done prep");
  LLVMValueRef global_module_initializer = cg_visit_module_statements(
      &code_generator, new_root, true, string_make("core"));
  cg_make_entrypoint(&code_generator, global_module_initializer);
  puts("Done code gen");

  // LLVMDumpModule(code_generator.module);

  char *error = NULL;
  if (LLVMPrintModuleToFile(code_generator.module, "out/compiled.ll", &error) !=
      0) {
    fprintf(stderr, "Failed to write module: %s", error);
    LLVMDisposeMessage(error);
  }

  printf("Arena Allocator used %ld/%ld (%0.2f%%) memory\n", allocator.length,
         allocator.capacity,
         (double)allocator.length / (double)allocator.capacity * 100.0);
  return 0;
}
