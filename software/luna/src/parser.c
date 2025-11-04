#include "parser.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser);
struct SymbolLiteralNode *parse_symbol_literal(struct Parser *parser);
struct DataType *parse_data_type(struct Parser *parser);
struct FunctionStatementNode *parse_function_statements(struct Parser *parser);
struct FunctionStatementNode *parse_function_statement(struct Parser *parser);

struct ScopedSymbolLiteralNode *
parse_scoped_symbol_literal(struct Parser *parser);

struct Parser parser_make(struct ArenaAllocator *allocator,
                          struct Token *tokens, uint16_t token_count) {
  return (struct Parser){
      .allocator = allocator,
      .tokens = tokens,
      .token_count = token_count,
      .position = 0,
  };
}

struct Token parser_peek(struct Parser *parser) {
  assert(parser->position < parser->token_count);

  return parser->tokens[parser->position];
}

struct FunctionCallExpressionNode *
parse_function_call_expression(struct Parser *parser,
                               struct ScopedSymbolLiteralNode *symbol) {

  assert(parser_peek(parser).type == T_LPAREN);
  parser->position++;

  struct FunctionCallArgumentExpressionsNode *root_argument = NULL;
  if (parser_peek(parser).type != T_RPAREN) {
    puts("function call may have arguments.");
    root_argument =
        ast_promote(parser->allocator,
                    &(struct FunctionCallArgumentExpressionsNode){
                        .argument = NULL,
                        .next = NULL,
                    },
                    sizeof(struct FunctionCallArgumentExpressionsNode));

    struct FunctionCallArgumentExpressionsNode *current_argument =
        root_argument;

    struct ExpressionNode *argument_expression = parse_expression(parser, 0);
    current_argument->argument = argument_expression;

    while (parser_peek(parser).type == T_COMMA) {
      parser->position++;
      argument_expression = parse_expression(parser, 0);
      current_argument->next =
          ast_promote(parser->allocator,
                      &(struct FunctionCallArgumentExpressionsNode){
                          .argument = argument_expression,
                          .next = NULL,
                      },
                      sizeof(struct FunctionCallArgumentExpressionsNode));
      current_argument = current_argument->next;
    }
  }

  assert(parser_peek(parser).type == T_RPAREN);
  parser->position++;

  return ast_promote(parser->allocator,
                     &(struct FunctionCallExpressionNode){
                         .name = symbol, .arguments = root_argument},
                     sizeof(struct FunctionCallExpressionNode));
}

uint8_t precedence_for_token(enum TokenType type) {
  switch (type) {
  case T_PLUS:
    return 1;
  case T_MINUS:
    return 1;
  case T_STAR:
    return 2;
  case T_SLASH:
    return 2;
  case T_LANGLE:
    return 3;
  case T_RANGLE:
    return 3;
  case T_EQEQ:
    return 3;
  case T_LEQ:
    return 3;
  case T_GEQ:
    return 3;
  case T_NTEQ:
    return 3;
  case T_RPAREN:
    return 0;
  case T_RBRACE:
    return 0;
  default:
    return 0;
  }
}

struct FunctionArgumentNode *parse_function_argument(struct Parser *parser) {
  struct SymbolLiteralNode *symbol = parse_symbol_literal(parser);
  assert(symbol != NULL);

  puts("taking the colon.");
  assert(parser_peek(parser).type == T_COLON);
  parser->position++;

  struct DataType *type = parse_data_type(parser);
  puts("took the type.");

  struct FunctionArgumentNode *next = NULL;
  if (parser_peek(parser).type == T_COMMA) {
    puts("found comma");
    parser->position++;
    next = parse_function_argument(parser);
  }
  puts("done.");
  printf("next token is type: %d\n", parser_peek(parser).type);

  return ast_promote(parser->allocator,
                     &(struct FunctionArgumentNode){.symbol = symbol->value,
                                                    .data_type = type,
                                                    .next = next},
                     sizeof(struct FunctionArgumentNode));
}

struct FunctionArgumentNode *parse_function_arguments(struct Parser *parser) {
  if (parser_peek(parser).type == T_RPAREN) {
    puts("function had no arguments");
    return NULL;
  }
  puts("Function may have arguments.");

  struct FunctionArgumentNode *arguments = parse_function_argument(parser);
  puts("Found arguments.");

  return arguments;
}

struct StructFieldDefinitionNode *
parse_struct_field_definition(struct Parser *parser) {
  if (parser_peek(parser).type != T_SYMBOL) {
    return NULL;
  }

  struct StructFieldDefinitionNode *field =
      ast_promote(parser->allocator,
                  &(struct StructFieldDefinitionNode){
                      .next = NULL,
                  },
                  sizeof(struct StructFieldDefinitionNode));

  struct LunaString name = parser_peek(parser).value.symbol;
  parser->position++;

  assert(parser_peek(parser).type == T_COLON);
  parser->position++;

  struct DataType *data_type = parse_data_type(parser);

  // TODO: May remove this comma
  if (parser_peek(parser).type == T_COMMA) {
    parser->position++;
  }

  field->name = name;
  field->type = data_type;
  field->next = parse_struct_field_definition(parser);
  return field;
}

struct StructFieldInitializerExpressionNode *
parse_struct_field_initializer(struct Parser *parser) {
  if (parser_peek(parser).type == T_RBRACE) {
    return NULL;
  }

  assert(parser_peek(parser).type == T_SYMBOL);
  struct LunaString name = parser_peek(parser).value.symbol;
  parser->position++;

  assert(parser_peek(parser).type == T_COLON);
  parser->position++;

  struct ExpressionNode *initialization_expression =
      parse_expression(parser, 0);

  if (parser_peek(parser).type == T_COMMA) {
    parser->position++;
  }

  struct StructFieldInitializerExpressionNode *next =
      parse_struct_field_initializer(parser);

  return ast_promote(parser->allocator,
                     &(struct StructFieldInitializerExpressionNode){
                         .name = name,
                         .expression = initialization_expression,
                         .next = next,
                     },
                     sizeof(struct StructFieldInitializerExpressionNode));
}

struct StructFieldAccessInnerExpressionNode *
parse_struct_field_access(struct Parser *parser) {
  if (parser_peek(parser).type != T_PERIOD) {
    return NULL;
  }
  parser->position++;

  assert(parser_peek(parser).type == T_SYMBOL);
  struct LunaString symbol = parser_peek(parser).value.symbol;
  printf("Got symbol: '%s'\n", symbol.data);
  parser->position++;

  return ast_promote(
      parser->allocator,
      &(struct StructFieldAccessInnerExpressionNode){
          .symbol = symbol, .next = parse_struct_field_access(parser)},
      sizeof(struct StructFieldAccessInnerExpressionNode));
}

struct ArrayInitializerExpressionNode *
parse_array_initializer(struct Parser *parser) {
  struct ExpressionNode *initializer = parse_expression(parser, 0);
  struct ArrayInitializerExpressionNode *next = NULL;
  if (parser_peek(parser).type == T_COMMA) {
    parser->position++;
    next = parse_array_initializer(parser);
  }
  return ast_promote(parser->allocator,
                     &(struct ArrayInitializerExpressionNode){
                         .initializer = initializer,
                         .next = next,
                     },
                     sizeof(struct ArrayInitializerExpressionNode));
}

struct ExpressionNode *parse_nud(struct Parser *parser, struct Token token) {
  switch (token.type) {
  case T_INTEGER:
    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){.type = EXPR_INTEGER_LITERAL,
                                .node.integer = parse_integer_literal(parser)});
  case T_SYMBOL: {
    struct ScopedSymbolLiteralNode *scoped_symbol =
        parse_scoped_symbol_literal(parser);

    switch (parser_peek(parser).type) {
    case T_LPAREN: {
      puts("found lparen, is function call");

      struct FunctionCallExpressionNode *fn_call =
          parse_function_call_expression(parser, scoped_symbol);
      assert(fn_call != NULL);

      return ast_promote(parser->allocator,
                         &(struct ExpressionNode){.type = EXPR_FN_CALL,
                                                  .node.fn_call = fn_call},
                         sizeof(struct ExpressionNode));
    }
    case T_PERIOD: {
      puts("found period, so this is field accessor");
      printf("scoped symbol: ");
      print_scoped_symbol(scoped_symbol);
      puts("");

      return ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_FIELD_ACCESS,
              .node.struct_field_access =
                  ast_promote(parser->allocator,
                              &(struct StructFieldAccessExpressionNode){
                                  .root_symbol = scoped_symbol,
                                  .next = parse_struct_field_access(parser)},
                              sizeof(struct StructFieldAccessExpressionNode))});
    }
    case T_LBRACK:
      puts("Found index operation");
      parser->position++;

      struct ExpressionNode *indexing_expr = parse_expression(parser, 0);

      assert(parser_peek(parser).type == T_RBRACK);
      parser->position++;

      return ast_promote(
          parser->allocator,
          &(struct ExpressionNode){
              .type = EXPR_DEREF,
              .node.deref = ast_promote_expression_node(
                  parser->allocator,
                  (struct ExpressionNode){
                      .type = EXPR_BINARY,
                      .node.binary = ast_make_binary_expression(
                          parser->allocator, BIN_EXPR_ADD,
                          ast_promote_expression_node(
                              parser->allocator,
                              (struct ExpressionNode){
                                  .type = EXPR_SYMBOL_LITERAL,
                                  .node.scoped_symbol = scoped_symbol}),
                          indexing_expr)})},
          sizeof(struct ExpressionNode));
    default:
      printf("did not find lparen after symbol, found: %d\n",
             parser_peek(parser).type);
      return ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){.type = EXPR_SYMBOL_LITERAL,
                                  .node.scoped_symbol = scoped_symbol});
    };
  }
  case T_STAR: {
    puts("parsing deref");
    parser->position++;

    struct ExpressionNode *deref_expr = parse_expression(parser, 0);

    return ast_promote(
        parser->allocator,
        &(struct ExpressionNode){.type = EXPR_DEREF, .node.deref = deref_expr},
        sizeof(struct ExpressionNode));
  }
  case T_AMPERSAND: {
    parser->position++;

    assert(parser_peek(parser).type == T_SYMBOL);
    struct ScopedSymbolLiteralNode *symbol =
        parse_scoped_symbol_literal(parser);

    puts("Looking for field access expr for ref");

    return ast_promote(parser->allocator,
                       &(struct ExpressionNode){
                           .type = EXPR_REF,
                           .node.ref_symbol = ast_promote(
                               parser->allocator,
                               &(struct StructFieldAccessExpressionNode){
                                   .root_symbol = symbol,
                                   .next = parse_struct_field_access(parser)},
                               sizeof(struct StructFieldAccessExpressionNode))},
                       sizeof(struct ExpressionNode));
  }
  case T_STRING: {
    parser->position++;

    return ast_promote(
        parser->allocator,
        &(struct ExpressionNode){
            .type = EXPR_STRING_LITERAL,
            .node.string = ast_promote(
                parser->allocator,
                &(struct StringLiteralNode){.value = token.value.symbol},
                sizeof(struct StringLiteralNode))},
        sizeof(struct ExpressionNode));
    break;
  }
  case T_VALUESIZE:
    parser->position++;
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;

    struct ExpressionNode *expr = parse_expression(parser, 0);

    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;

    return ast_promote_expression_node(
        parser->allocator, (struct ExpressionNode){.type = EXPR_VALUESIZE,
                                                   .node.valuesize = expr});
  case T_TYPESIZE:
    parser->position++;
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;

    struct DataType *dt = parse_data_type(parser);

    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;

    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){.type = EXPR_TYPESIZE, .node.typesize = dt});
  case T_CAST: {
    parser->position++;

    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;

    struct DataType *type = parse_data_type(parser);

    assert(parser_peek(parser).type == T_COMMA);
    parser->position++;

    struct ExpressionNode *expr = parse_expression(parser, 0);

    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;

    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){
            .type = EXPR_CAST,
            .node.cast = ast_promote(
                parser->allocator,
                &(struct CastExpressionNode){.expr = expr, .type = type},
                sizeof(struct CastExpressionNode))});
  }
  case T_LPAREN: {
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    struct ExpressionNode *result = parse_expression(parser, 0);
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++; // consume the close paren.
    return result;
  }
  case T_FN: {
    struct DataType *return_type = NULL;
    parser->position++;
    struct LunaString *extern_name = NULL;
    bool is_variadic = false;
    if (parser_peek(parser).type == T_EXTERN) {
      parser->position++;

      assert(parser_peek(parser).type == T_LBRACK);
      parser->position++;

      assert(parser_peek(parser).type == T_STRING);
      struct LunaString name = parser_peek(parser).value.symbol;
      extern_name =
          ast_promote(parser->allocator, &name, sizeof(struct LunaString));
      parser->position++;

      assert(parser_peek(parser).type == T_RBRACK);
      parser->position++;

      if (parser_peek(parser).type == T_VARIADIC) {
        parser->position++;
        is_variadic = true;
      }
    }
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    struct FunctionArgumentNode *arguments = parse_function_arguments(parser);
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;
    printf("a. next token is: %d\n", parser_peek(parser).type);
    if (parser_peek(parser).type == T_COLON) {
      puts("Found colon in func");
      parser->position++;
      return_type = parse_data_type(parser);
    } else {
      return_type = make_void_data_type(parser->allocator);
      puts("Found NO colon in func");
    }
    printf("next token is: %d\n", parser_peek(parser).type);
    if (extern_name != NULL) {
      return ast_promote(
          parser->allocator,
          &(struct ExpressionNode){
              .type = EXPR_FN_DEF,
              .node.fn_def =
                  ast_promote(parser->allocator,
                              &(struct FunctionDefinitionExpressionNode){
                                  .body = NULL,
                                  .function_type = make_function_data_type(
                                      parser->allocator, arguments, return_type,
                                      extern_name, is_variadic),
                              },
                              sizeof(struct FunctionDefinitionExpressionNode)),
          },
          sizeof(struct ExpressionNode));
    }
    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;
    struct FunctionStatementNode *result = parse_function_statements(parser);
    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++; // consume the close brace.
    printf("return type IS NULL?: %d\n", return_type == NULL);
    return ast_promote(parser->allocator,
                       &(struct ExpressionNode){
                           .type = EXPR_FN_DEF,
                           .node.fn_def = ast_promote(
                               parser->allocator,
                               &(struct FunctionDefinitionExpressionNode){
                                   .body = result,
                                   .function_type = make_function_data_type(
                                       parser->allocator, arguments,
                                       return_type, NULL, is_variadic),
                               },
                               sizeof(struct FunctionDefinitionExpressionNode)),
                       },
                       sizeof(struct ExpressionNode));
  }
  case T_PERIOD: {
    parser->position++;
    puts("found struct initialization");

    struct ScopedSymbolLiteralNode *scoped_symbol =
        parse_scoped_symbol_literal(parser);

    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;

    struct StructFieldInitializerExpressionNode *fields =
        parse_struct_field_initializer(parser);

    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++; // consume the close brace.

    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){
            .type = EXPR_STRUCT_INIT,
            .node.struct_init = ast_promote(
                parser->allocator,
                &(struct StructInitializationExpressionNode){
                    .name = scoped_symbol,
                    .fields = fields,

                },
                sizeof(struct StructInitializationExpressionNode))});
  }

  case T_STRUCT: {
    parser->position++;

    struct StructDefinitionExpressionNode struct_def;

    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;

    struct_def.fields = parse_struct_field_definition(parser);

    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++;

    return ast_promote(parser->allocator,
                       &(struct ExpressionNode){
                           .type = EXPR_STRUCT_DEF,
                           .node.struct_def = ast_promote(
                               parser->allocator, &struct_def,
                               sizeof(struct StructDefinitionExpressionNode)),
                       },
                       sizeof(struct ExpressionNode));
  }
  case T_MOD: {
    puts("Found module definition");
    parser->position++;

    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;

    struct ModuleStatementNode *module_statements =
        parse_module_statements(parser);

    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++;

    return ast_promote(parser->allocator,
                       &(struct ExpressionNode){
                           .type = EXPR_MOD_DEF,
                           .node.module_definition =
                               ast_promote(parser->allocator,
                                           &(struct ModuleNode){
                                               .statements = module_statements,
                                           },
                                           sizeof(struct ModuleNode)),
                       },
                       sizeof(struct ExpressionNode));

    break;
  }
  case T_LBRACK:
    puts("Found array initializer.");
    parser->position++;

    struct ArrayInitializerExpressionNode *initializers =
        parse_array_initializer(parser);

    assert(parser_peek(parser).type == T_RBRACK);
    parser->position++;

    return ast_promote_expression_node(
        parser->allocator, (struct ExpressionNode){
                               .type = EXPR_ARRAY_INITIALIZER,
                               .node.array_initializers = initializers,
                           });
  default:
    printf("Found type: %d\n", token.type);
    assert(false);
  }
}

struct ExpressionNode *parse_expression(struct Parser *parser,
                                        uint8_t precendence) {
  struct Token token = parser_peek(parser);

  struct ExpressionNode *left = parse_nud(parser, token);

  while (precendence < precedence_for_token(parser_peek(parser).type)) {
    token = parser_peek(parser);
    if (token.type == T_EOF) {
      return left;
    }

    parser->position++;

    switch (token.type) {
    case T_PLUS:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_ADD, left,
                  parse_expression(parser, precedence_for_token(T_PLUS)))});
      break;
    case T_MINUS:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_SUB, left,
                  parse_expression(parser, precedence_for_token(T_MINUS)))});
      break;
    case T_STAR:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_MUL, left,
                  parse_expression(parser, precedence_for_token(T_STAR)))});
      break;
    case T_SLASH:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_DIV, left,
                  parse_expression(parser, precedence_for_token(T_SLASH)))});
      break;
    case T_LANGLE:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_LT, left,
                  parse_expression(parser, precedence_for_token(T_LANGLE)))});
      break;
    case T_RANGLE:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_GT, left,
                  parse_expression(parser, precedence_for_token(T_RANGLE)))});
      break;
    case T_EQEQ:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_EQ, left,
                  parse_expression(parser, precedence_for_token(T_EQEQ)))});
      break;
    case T_NTEQ:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_NEQ, left,
                  parse_expression(parser, precedence_for_token(T_NTEQ)))});
      break;
    case T_LEQ:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_LEQ, left,
                  parse_expression(parser, precedence_for_token(T_LEQ)))});
      break;
    case T_GEQ:
      left = ast_promote_expression_node(
          parser->allocator,
          (struct ExpressionNode){
              .type = EXPR_BINARY,
              .node.binary = ast_make_binary_expression(
                  parser->allocator, BIN_EXPR_GEQ, left,
                  parse_expression(parser, precedence_for_token(T_GEQ)))});
      break;
    default:
      printf("Found unexpected token of type %d\n", token.type);
      assert(0);
      return left;
    }
  }

  return left;
}

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser) {
  struct Token token = parser_peek(parser);
  assert(token.type == T_INTEGER);

  parser->position++;

  return ast_make_integer_literal(parser->allocator, token.value.integer);
}

struct SymbolLiteralNode *parse_symbol_literal(struct Parser *parser) {
  struct Token token = parser_peek(parser);
  assert(token.type == T_SYMBOL);

  parser->position++;

  return ast_promote(parser->allocator,
                     &(struct SymbolLiteralNode){.value = token.value.symbol},
                     sizeof(struct SymbolLiteralNode));
}

struct ScopedSymbolLiteralNode *
parse_scoped_symbol_literal(struct Parser *parser) {
  assert(parser_peek(parser).type == T_SYMBOL);

  struct SymbolLiteralNode *symbol = parse_symbol_literal(parser);

  if (parser_peek(parser).type == T_COLON) {
    parser->position++;
    assert(parser_peek(parser).type == T_COLON);
    parser->position++;

    struct ScopedSymbolLiteralNode *next = parse_scoped_symbol_literal(parser);
    return ast_promote(parser->allocator,
                       &(struct ScopedSymbolLiteralNode){
                           .symbol = symbol,
                           .next = next,
                           .remaining = 1 + next->remaining,
                       },
                       sizeof(struct ScopedSymbolLiteralNode));
  }

  return ast_promote(parser->allocator,
                     &(struct ScopedSymbolLiteralNode){
                         .symbol = symbol,
                         .next = NULL,
                         .remaining = 1,
                     },
                     sizeof(struct ScopedSymbolLiteralNode));
}

struct ModuleStatementNode *parse_module_statements(struct Parser *parser) {
  assert(parser_peek(parser).type != T_EOF);

  struct ModuleStatementNode *head = parse_module_statement(parser);
  struct ModuleStatementNode *curr = head;

  enum TokenType peak;
  while ((peak = parser_peek(parser).type, peak != T_RBRACE && peak != T_EOF)) {
    curr->next = parse_module_statement(parser);
    curr = curr->next;
  }

  return head;
}

struct FunctionStatementNode *parse_function_statements(struct Parser *parser) {
  assert(parser_peek(parser).type != T_EOF);

  if (parser_peek(parser).type == T_RBRACE) {
    puts("Found empty function");
    return NULL;
  }

  struct FunctionStatementNode *head = parse_function_statement(parser);
  struct FunctionStatementNode *curr = head;

  enum TokenType peak;
  while ((peak = parser_peek(parser).type, peak != T_RBRACE && peak != T_EOF)) {
    curr->next = parse_function_statement(parser);
    curr = curr->next;
  }

  return head;
}

struct DataType *parse_data_type(struct Parser *parser) {
  struct Token token = parser_peek(parser);
  switch (token.type) {
  case T_STAR: {
    puts("\n\t\tFOUND POINTER TYPE!\n");
    parser->position++;
    struct DataType *inner = parse_data_type(parser);

    assert(inner != NULL);

    struct DataType *data_type = ast_promote(parser->allocator,
                                             &(struct DataType){
                                                 .kind = DTK_POINTER,
                                                 .value.pointer_inner = inner,
                                                 .next = NULL,
                                             },
                                             sizeof(struct DataType));
    printf("parsed data type: ");
    print_data_type(data_type);
    printf("\n");
    return data_type;
  }
  case T_SYMBOL: {
    if (strncmp("i8", token.value.symbol.data, 2) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 8, 1);
    } else if (strncmp("i16", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 16, 1);
    } else if (strncmp("i32", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 32, 1);
    } else if (strncmp("i64", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 64, 1);
    } else if (strncmp("u8", token.value.symbol.data, 2) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 8, 0);
    } else if (strncmp("u16", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 16, 0);
    } else if (strncmp("u32", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 32, 0);
    } else if (strncmp("u64", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 64, 0);
    } else if (strncmp("i1", token.value.symbol.data, 2) == 0) {
      parser->position++;
      return make_integer_primitive_data_type(parser->allocator, 1, 1);
    } else if (strncmp("void", token.value.symbol.data, 4) == 0) {
      printf("\n\n\tVOID\n");
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct DataType){
                             .kind = DTK_VOID,
                             .next = NULL,
                         },
                         sizeof(struct DataType));
    } else {
      printf("assuming unknown symbol is structure: %s\n",
             token.value.symbol.data);
      struct ScopedSymbolLiteralNode *scoped_symbol =
          parse_scoped_symbol_literal(parser);

      return make_structure_data_type(
          parser->allocator,
          (struct StructType){.name = scoped_symbol, .definition = NULL});
    }
    break;
  }
  case T_FN: {
    parser->position++;

    struct LunaString *extern_name = NULL;
    bool is_variadic = false;
    if (parser_peek(parser).type == T_EXTERN) {
      parser->position++;

      assert(parser_peek(parser).type == T_LBRACK);
      parser->position++;

      assert(parser_peek(parser).type == T_STRING);
      struct LunaString name = parser_peek(parser).value.symbol;
      extern_name =
          ast_promote(parser->allocator, &name, sizeof(struct LunaString));
      parser->position++;

      assert(parser_peek(parser).type == T_RBRACK);
      parser->position++;

      if (parser_peek(parser).type == T_VARIADIC) {
        parser->position++;
        is_variadic = true;
      }
    }
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    struct FunctionArgumentNode *arguments = parse_function_arguments(parser);
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;
    struct DataType *return_type = NULL;
    if (parser_peek(parser).type == T_COLON) {
      puts("function had return type");
      parser->position++;
      return_type = parse_data_type(parser);
      printf("got return type: %d\n", return_type->kind);
    } else {
      puts("function had NO return type");
      return_type = make_void_data_type(parser->allocator);
    }

    puts("parsed function type here");
    struct DataType *type = make_function_data_type(
        parser->allocator, arguments, return_type, extern_name, is_variadic);
    printf("return type: is null: %d\n", type == NULL);
    return type;
  }
  case T_LBRACK:
    puts("Found array type.");
    parser->position++;

    struct DataType *element_type = parse_data_type(parser);

    assert(parser_peek(parser).type == T_SEMICOLON);
    parser->position++;

    printf("About to parse integer for array length, next token is type: %d\n",
           parser_peek(parser).type);

    uint64_t length = parse_integer_literal(parser)->value;

    assert(parser_peek(parser).type == T_RBRACK);
    parser->position++;

    return make_array_data_type(parser->allocator, element_type, length);
  default:
    assert(0);
  }

  return NULL;
}

struct DeclarationStatementNode *parse_decl_statement(struct Parser *parser,
                                                      bool is_const) {
  assert(parser_peek(parser).type == (is_const ? T_CONST : T_LET));
  parser->position++;

  assert(parser_peek(parser).type == T_SYMBOL);
  struct LunaString symbol = parser_peek(parser).value.symbol;
  parser->position++;

  struct DataType *data_type = NULL;
  bool has_type = false;
  if (parser_peek(parser).type == T_COLON) {
    parser->position++;

    data_type = parse_data_type(parser);
    has_type = true;
  }

  printf("next tok is now: %d\n", parser_peek(parser).type);

  assert(parser_peek(parser).type == T_EQUALS);
  parser->position++;

  struct ExpressionNode *expr = parse_expression(parser, 0);

  assert(parser_peek(parser).type == T_SEMICOLON);
  parser->position++;

  return ast_promote(parser->allocator,
                     &(struct DeclarationStatementNode){.symbol = symbol,
                                                        .expression = expr,
                                                        .is_const = is_const,
                                                        .has_type = has_type,
                                                        .data_type = data_type},
                     sizeof(struct DeclarationStatementNode));
}

struct ReturnStatementNode *parse_ret_statement(struct Parser *parser) {
  if (parser_peek(parser).type == T_SEMICOLON) {

    return ast_promote(parser->allocator,
                       &(struct ReturnStatementNode){.expression = NULL},
                       sizeof(struct DeclarationStatementNode));
  }
  return ast_promote(
      parser->allocator,
      &(struct ReturnStatementNode){.expression = parse_expression(parser, 0)},
      sizeof(struct DeclarationStatementNode));
}

struct FunctionStatementNode *parse_function_statement(struct Parser *parser) {
  switch (parser_peek(parser).type) {
  case T_LET: {
    struct DeclarationStatementNode *decl = parse_decl_statement(parser, false);
    return ast_promote(parser->allocator,
                       &(struct FunctionStatementNode){
                           .type = FN_STMT_LET,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct FunctionStatementNode));
  }
  case T_CONST: {
    struct DeclarationStatementNode *decl = parse_decl_statement(parser, true);
    return ast_promote(parser->allocator,
                       &(struct FunctionStatementNode){
                           .type = FN_STMT_CONST,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct FunctionStatementNode));
  }
  case T_RETURN: {
    puts("found return.");
    parser->position++;
    struct ReturnStatementNode *ret = parse_ret_statement(parser);

    assert(parser_peek(parser).type == T_SEMICOLON);
    parser->position++;

    puts("parsed ret");
    return ast_promote(parser->allocator,
                       &(struct FunctionStatementNode){.type = FN_STMT_RETURN,
                                                       .node.ret = ret,
                                                       .next = NULL},
                       sizeof(struct FunctionStatementNode));
  }
  case T_WHILE: {
    puts("found while.");
    printf("\n\n\n-------------------------------------------------------------"
           "---\n\n");
    parser->position++;

    struct WhileStatementNode while_stmt =
        (struct WhileStatementNode){.body = NULL, .condition = NULL};

    struct ExpressionNode *conditional = parse_expression(parser, 0);

    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;

    struct FunctionStatementNode *body = parse_function_statements(parser);

    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++;

    while_stmt.condition = conditional;
    while_stmt.body = body;

    assert(parser_peek(parser).type == T_SEMICOLON);
    parser->position++;

    return ast_promote(
        parser->allocator,
        &(struct FunctionStatementNode){
            .type = FN_STMT_WHILE,
            .node.while_stmt = ast_promote(parser->allocator, &while_stmt,
                                           sizeof(struct WhileStatementNode))},
        sizeof(struct FunctionStatementNode));
  }
  case T_IF: {
    puts("found if.");
    printf("\n\n\n-------------------------------------------------------------"
           "---\n\n");
    parser->position++;

    struct IfStatementNode if_stmt =
        (struct IfStatementNode){.next = NULL, .body = NULL, .condition = NULL};

    struct IfStatementNode *current = &if_stmt;

    puts("about to check if statemtn stuff.");
    while (true) {
      puts("starting layer");
      struct ExpressionNode *conditional = parse_expression(parser, 0);

      assert(parser_peek(parser).type == T_LBRACE);
      parser->position++;
      struct FunctionStatementNode *body = parse_function_statements(parser);

      assert(parser_peek(parser).type == T_RBRACE);
      parser->position++;

      current->condition = conditional;
      current->body = body;
      if (parser_peek(parser).type == T_ELSE) {
        parser->position++;
        if (parser_peek(parser).type == T_IF) {
          puts("found else if case");
          parser->position++;
          current->next =
              ast_promote(parser->allocator,
                          &(struct IfStatementNode){
                              .next = NULL, .body = NULL, .condition = NULL},
                          sizeof(struct IfStatementNode));
          current = current->next;
          continue;
        } else {
          puts("found else not if case");
          assert(parser_peek(parser).type == T_LBRACE);
          parser->position++;
          struct FunctionStatementNode *body =
              parse_function_statements(parser);

          assert(parser_peek(parser).type == T_RBRACE);
          parser->position++;

          current->next =
              ast_promote(parser->allocator,
                          &(struct IfStatementNode){
                              .next = NULL, .body = body, .condition = NULL},
                          sizeof(struct IfStatementNode));
          current = current->next;
        }
        break;
      } else {
        puts("no else found.");
        break;
      }
    }

    assert(parser_peek(parser).type == T_SEMICOLON);
    parser->position++;

    return ast_promote(
        parser->allocator,
        &(struct FunctionStatementNode){
            .type = FN_STMT_IF,
            .next = NULL,
            .node.if_stmt = ast_promote(parser->allocator, &if_stmt,
                                        sizeof(struct IfStatementNode))},
        sizeof(struct FunctionStatementNode));
  }
  default: {
    puts("[PARSER] In default case for function statement.");
    printf("Looking at: %d\n", parser_peek(parser).type);
    struct ExpressionNode *expr = parse_expression(parser, 0);

    assert(expr != NULL);
    printf("[PARSER] found expression of type: %d\n", expr->type);

    switch (expr->type) {
    case EXPR_FN_CALL:
      assert(expr->node.fn_call != NULL);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct FunctionStatementNode){
                             .type = FN_STMT_FN_CALL,
                             .node.fn_call = expr->node.fn_call,
                             .next = NULL,

                         },
                         sizeof(struct FunctionStatementNode));
    case EXPR_DEREF: {
      assert(parser_peek(parser).type == T_EQUALS);
      parser->position++;

      struct ExpressionNode *result_expression = parse_expression(parser, 0);
      assert(result_expression != NULL);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct FunctionStatementNode){
                             .type = FN_STMT_ASSIGN,
                             .node.assign = ast_promote(
                                 parser->allocator,
                                 &(struct AssignStatementNode){
                                     .source_expression = expr,
                                     .result_expression = result_expression},
                                 sizeof(struct AssignStatementNode)),
                             .next = NULL,
                         },
                         sizeof(struct FunctionStatementNode));
    }
    case EXPR_FIELD_ACCESS:
      assert(parser_peek(parser).type == T_EQUALS);
      parser->position++;

      struct ExpressionNode *result_expression = parse_expression(parser, 0);
      assert(result_expression != NULL);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct FunctionStatementNode){
                             .type = FN_STMT_ASSIGN,
                             .node.assign = ast_promote(
                                 parser->allocator,
                                 &(struct AssignStatementNode){
                                     .source_expression = expr,
                                     .result_expression = result_expression},
                                 sizeof(struct AssignStatementNode)),
                             .next = NULL,
                         },
                         sizeof(struct FunctionStatementNode));
    case EXPR_SYMBOL_LITERAL: {
      puts("plain symbol assignment.");

      assert(parser_peek(parser).type == T_EQUALS);
      parser->position++;

      struct ExpressionNode *result_expression = parse_expression(parser, 0);

      printf("in assign, parsed expression of type: %d\n",
             result_expression->type);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct FunctionStatementNode){
                             .type = FN_STMT_ASSIGN,
                             .node.assign = ast_promote(
                                 parser->allocator,
                                 &(struct AssignStatementNode){
                                     .source_expression = expr,
                                     .result_expression = result_expression},
                                 sizeof(struct AssignStatementNode)),
                             .next = NULL,
                         },
                         sizeof(struct FunctionStatementNode));
    }
    case EXPR_ARRAY_INITIALIZER:
      puts("Found array initializer fn stmt");
      assert(0);

      return ast_promote(
          parser->allocator,
          &(struct FunctionStatementNode){
              .type = FN_STMT_ASSIGN,
              .node.assign = ast_promote(
                  parser->allocator,
                  &(struct AssignStatementNode){
                      .source_expression = ast_promote(
                          parser->allocator,
                          &(struct ExpressionNode){.type = EXPR_DEREF,
                                                   .node.deref = expr},
                          sizeof(struct ExpressionNode)),
                      .result_expression = result_expression},
                  sizeof(struct AssignStatementNode)),
              .next = NULL,
          },
          sizeof(struct FunctionStatementNode));
      break;
    case EXPR_BINARY:
    case EXPR_INTEGER_LITERAL:
    case EXPR_STRING_LITERAL:
    case EXPR_FN_DEF:
    case EXPR_STRUCT_DEF:
    case EXPR_STRUCT_INIT:
    case EXPR_REF:
    case EXPR_CAST:
    case EXPR_MOD_DEF:
    case EXPR_VALUESIZE:
    case EXPR_TYPESIZE:
      puts("illegal expression type as function statement.");
      assert(0);
      return NULL;
    }
  }
  };
  puts("illegal function statement.");
  assert(0);
  return NULL;
}

struct ModuleStatementNode *parse_module_statement(struct Parser *parser) {
  switch (parser_peek(parser).type) {
  case T_LET: {
    struct DeclarationStatementNode *decl = parse_decl_statement(parser, false);
    return ast_promote(parser->allocator,
                       &(struct ModuleStatementNode){
                           .type = MOD_STMT_LET,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct ModuleStatementNode));
  }
  case T_CONST: {
    struct DeclarationStatementNode *decl = parse_decl_statement(parser, true);
    return ast_promote(parser->allocator,
                       &(struct ModuleStatementNode){
                           .type = MOD_STMT_CONST,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct ModuleStatementNode));
  }
  default: {
    puts("Unexpected token in module.");
    assert(0);
    break;
  }
  }
  assert(false);
  return NULL;
}
