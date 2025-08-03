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
  case T_RPAREN:
    return 0;
  case T_RBRACE:
    return 0;
  default:
    return 0;
  }
}

struct ExpressionNode *parse_nud(struct Parser *parser, struct Token token) {
  switch (token.type) {
  case T_INTEGER:
    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){.type = EXPR_INTEGER_LITERAL,
                                .node.integer = parse_integer_literal(parser)});
  case T_SYMBOL:
    return ast_promote_expression_node(
        parser->allocator,
        (struct ExpressionNode){.type = EXPR_SYMBOL_LITERAL,
                                .node.symbol = parse_symbol_literal(parser)});
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
    assert(parser_peek(parser).type == T_FN);
    parser->position++;
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++;
    if (parser_peek(parser).type == T_COLON) {
      puts("Found colon in func");
      parser->position++;
      return_type = parse_data_type(parser);
    } else {
      puts("Found NO colon in func");
    }
    assert(parser_peek(parser).type == T_LBRACE);
    parser->position++;
    struct FunctionStatementNode *result = parse_function_statements(parser);
    assert(parser_peek(parser).type == T_RBRACE);
    parser->position++; // consume the close brace.
    return ast_promote(parser->allocator,
                       &(struct ExpressionNode){
                           .type = EXPR_FN_DEF,
                           .node.fn_def = ast_promote(
                               parser->allocator,
                               &(struct FunctionDefinitionExpressionNode){
                                   .body = result,
                                   .return_type = return_type,
                               },
                               sizeof(struct FunctionDefinitionExpressionNode)),
                       },
                       sizeof(struct ExpressionNode));
  }
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
    default:
      printf("Found unexpected token of type %d\n", token.type);
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
  case T_SYMBOL: {
    if (strncmp("int", token.value.symbol.data, 3) == 0) {
      parser->position++;
      return ast_promote(parser->allocator,
                         &(struct DataType){
                             .kind = DTK_PRIMITIVE,
                             .value.primitive = P_INT,
                             .next = NULL,
                         },
                         sizeof(struct DataType));
    } else if (strncmp("bool", token.value.symbol.data, 4) == 0) {
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct DataType){
                             .kind = DTK_PRIMITIVE,
                             .value.primitive = P_BOOL,
                             .next = NULL,
                         },
                         sizeof(struct DataType));
    } else {
      printf("Unknown primitive data type: %s\n", token.value.symbol.data);
      assert(0);
    }
    break;
  }
  case T_FN: {
    break;
  }
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
  default: {
    printf("Looking at: %d\n", parser_peek(parser).type);
    assert(parser_peek(parser).type == T_SYMBOL);
    struct LunaString symbol = parser_peek(parser).value.symbol;
    parser->position++;

    switch (parser_peek(parser).type) {
    case T_EQUALS: {
      parser->position++;
      struct ExpressionNode *expr = parse_expression(parser, 0);

      printf("in assign, parsed expression of type: %d\n", expr->type);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct FunctionStatementNode){
                             .type = FN_STMT_ASSIGN,
                             .node.assign = ast_promote(
                                 parser->allocator,
                                 &(struct AssignStatementNode){
                                     .symbol = symbol, .expression = expr},
                                 sizeof(struct AssignStatementNode)),
                             .next = NULL,
                         },
                         sizeof(struct FunctionStatementNode));
      break;
    }
    default:
      assert(0);
      break;
    };
  }
  }
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
    assert(0);
    break;
  }
  }
  assert(false);
  return NULL;
}
