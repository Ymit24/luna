#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "luna_string.h"

struct Variable {
  struct LunaString symbol;
  uint16_t value;
  struct Variable *next;
  bool is_const;
};

struct Environment {
  struct Variable *variable;
  struct ArenaAllocator *allocator;
};

void evaluate_statements(struct Environment *environment,
                         struct StatementNode *stmt);

uint16_t evaluate_expression(struct Environment *environment,
                             struct ExpressionNode *expr);

#endif
