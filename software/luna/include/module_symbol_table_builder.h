#ifndef MODULE_SYMBOL_TABLE_BUILDER_H
#define MODULE_SYMBOL_TABLE_BUILDER_H

#include "annotator.h"
#include "ast.h"

struct QueueItem {
  void *data;
  struct QueueItem *next;
};

struct Queue {
  struct QueueItem *head;
  struct QueueItem *tail;
};

void mstb_visit_module(struct Annotator *annotator,
                       struct ModuleStatementNode *root);
void mstb_infer_types(struct Annotator *annotator,
                      struct SymbolTable *symbol_table);

struct QueueItem *queue_item_make(struct ArenaAllocator *allocator, void *data);

struct Queue *queue_make(struct ArenaAllocator *allocator);
void queue_push(struct ArenaAllocator *allocator, struct Queue *queue,
                void *data);

void *queue_pop_head(struct Queue *queue);


void mstb_resolve_types(struct Annotator *annotator, struct DataType *type);
#endif
