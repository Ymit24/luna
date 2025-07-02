#include "parser.h"
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"
#include "token.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

struct IntegerLiteralNode *parse_integer_literal(struct Parser *parser);
struct SymbolLiteralNode *parse_symbol_literal(struct Parser *parser);

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
  case T_LPAREN:
    assert(parser_peek(parser).type == T_LPAREN);
    parser->position++;
    struct ExpressionNode *result = parse_expression(parser, 0);
    assert(parser_peek(parser).type == T_RPAREN);
    parser->position++; // consume the close paren.
    return result;
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

struct StatementNode *parse_statements(struct Parser *parser) {
  assert(parser_peek(parser).type != T_EOF);

  struct StatementNode *head = parse_statement(parser);
  struct StatementNode *curr = head;

  while (parser_peek(parser).type != T_EOF) {
    curr->next = parse_statement(parser);
    curr = curr->next;
  }

  return head;
}

struct DeclarationStatementNode *parse_let_statement(struct Parser *parser) {
  assert(parser_peek(parser).type == T_LET);
  parser->position++;

  assert(parser_peek(parser).type == T_SYMBOL);
  struct LunaString symbol = parser_peek(parser).value.symbol;
  parser->position++;

  assert(parser_peek(parser).type == T_EQUALS);
  parser->position++;

  struct ExpressionNode *expr = parse_expression(parser, 0);

  assert(parser_peek(parser).type == T_SEMICOLON);
  parser->position++;

  return ast_promote(parser->allocator,
                     &(struct DeclarationStatementNode){
                         .symbol = symbol,
                         .expression = expr,
                         .is_const = false,
                     },
                     sizeof(struct DeclarationStatementNode));
}

struct DeclarationStatementNode *parse_const_statement(struct Parser *parser) {
  assert(parser_peek(parser).type == T_CONST);
  parser->position++;

  assert(parser_peek(parser).type == T_SYMBOL);
  struct LunaString symbol = parser_peek(parser).value.symbol;
  parser->position++;

  assert(parser_peek(parser).type == T_EQUALS);
  parser->position++;

  struct ExpressionNode *expr = parse_expression(parser, 0);

  assert(parser_peek(parser).type == T_SEMICOLON);
  parser->position++;

  return ast_promote(parser->allocator,
                     &(struct DeclarationStatementNode){
                         .symbol = symbol,
                         .expression = expr,
                         .is_const = true,
                     },
                     sizeof(struct DeclarationStatementNode));
}

struct StatementNode *parse_statement(struct Parser *parser) {
  switch (parser_peek(parser).type) {
  case T_LET: {
    struct DeclarationStatementNode *decl = parse_let_statement(parser);
    return ast_promote(parser->allocator,
                       &(struct StatementNode){
                           .type = STMT_LET,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct StatementNode));
  }
  case T_CONST: {
    struct DeclarationStatementNode *decl = parse_const_statement(parser);
    return ast_promote(parser->allocator,
                       &(struct StatementNode){
                           .type = STMT_CONST,
                           .node.decl = decl,
                           .next = NULL,
                       },
                       sizeof(struct StatementNode));
  }
  default: {
    assert(parser_peek(parser).type == T_SYMBOL);
    struct LunaString symbol = parser_peek(parser).value.symbol;
    parser->position++;

    switch (parser_peek(parser).type) {
    case T_EQUALS: {
      parser->position++;
      struct ExpressionNode *expr = parse_expression(parser, 0);

      assert(parser_peek(parser).type == T_SEMICOLON);
      parser->position++;

      return ast_promote(parser->allocator,
                         &(struct StatementNode){
                             .type = STMT_ASSIGN,
                             .node.assign = ast_promote(
                                 parser->allocator,
                                 &(struct AssignStatementNode){
                                     .symbol = symbol, .expression = expr},
                                 sizeof(struct AssignStatementNode)),
                             .next = NULL,
                         },
                         sizeof(struct StatementNode));
      break;
    }
    default:
      assert(0);
      break;
    };
  }
  }
  assert(false);
  return NULL;
}
