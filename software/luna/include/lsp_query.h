#ifndef LSP_QUERY_H
#define LSP_QUERY_H

#include <stdint.h>
#include "arena_allocator.h"
#include "ast.h"
#include "luna_string.h"

enum ContainingNodeType {
  NODE_NONE,
  NODE_MODULE,
  NODE_FUNCTION,
  NODE_VARIABLE,
  NODE_EXPRESSION,
};

struct ContainingNode {
  enum ContainingNodeType type;
  struct SourceLocation location;
  struct LunaString name;
  union {
    struct ModuleNode *module;
    struct FunctionDefinitionExpressionNode *function;
    struct DeclarationStatementNode *variable;
    struct ExpressionNode *expression;
  } node;
};

struct LineMap {
  uint16_t *line_offsets;
  uint16_t num_lines;
};

struct LineMap build_line_map(struct LunaString source,
                             struct ArenaAllocator *alloc);

uint16_t line_col_to_byte_offset(struct LineMap map, uint16_t line,
                                uint16_t col,
                                struct LunaString source);

struct ContainingNode
find_smallest_containing_node(struct ModuleStatementNode *ast,
                               uint16_t byte_offset);

const char *node_type_to_string(enum ContainingNodeType type);

void query_hover_info(struct ContainingNode node, char *out_buf, size_t max_len);

#endif
