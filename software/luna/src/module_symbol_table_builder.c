#include "module_symbol_table_builder.h"
#include "annotator.h"
#include "arena_allocator.h"
#include "ast.h"
#include <assert.h>
#include <stdio.h>

struct QueueItem *queue_item_make(struct ArenaAllocator *allocator,
                                  void *data) {
  return ast_promote(allocator, &(struct QueueItem){.data = data, .next = NULL},
                     sizeof(struct QueueItem));
}

struct Queue *queue_make(struct ArenaAllocator *allocator) {
  return ast_promote(allocator, &(struct Queue){.head = NULL, .tail = NULL},
                     sizeof(struct Queue));
}

void queue_push(struct ArenaAllocator *allocator, struct Queue *queue,
                void *data) {
  struct QueueItem *item = queue_item_make(allocator, data);

  if (queue->head == NULL) {
    queue->head = item;
    queue->tail = item;
    return;
  }

  queue->tail->next = item;
  queue->tail = item;
}

void *queue_pop_head(struct Queue *queue) {
  struct QueueItem *head = queue->head;

  if (queue->head == queue->tail) {
    queue->head = NULL;
    queue->tail = NULL;
  }

  if (head != NULL) {
    queue->head = head->next;
    if (queue->head == NULL) {
      queue->head = queue->tail;
    }
    head->next = NULL;
  }

  return head->data;
}

void mstb_visit_module(struct Annotator *annotator,
                       struct ModuleStatementNode *root) {
  assert(annotator != NULL);
  assert(root != NULL);

  puts("Creating queue..");
  struct Queue *queue = queue_make(annotator->allocator);

  root->node.decl->expression->node.module_definition->symbol_table =
      (struct SymbolTable){.head = NULL,
                           .type = STT_MOD,
                           .current_index = 0,
                           .parent = &annotator->root_symbol_table};

  root->node.decl->data_type = infer_type(annotator, root->node.decl->expression);

  queue_push(annotator->allocator, queue, root);

  while (queue->head != NULL) {
    struct ModuleStatementNode *head = queue_pop_head(queue);

    assert(head != NULL);
    {
      printf("[mstb_visit_module] Found mod decl: '%s' of type: '",
             head->node.decl->symbol.data);
      print_data_type(head->node.decl->data_type);
      printf("'\n");
    }

    if (head->node.decl->expression->type == EXPR_MOD_DEF) {
      struct ModuleStatementNode *stmt =
          head->node.decl->expression->node.module_definition->statements;
      while (stmt != NULL) {
        if (stmt->node.decl->expression->type == EXPR_MOD_DEF) {
          stmt->node.decl->expression->node.module_definition->symbol_table =
              (struct SymbolTable){.head = NULL,
                                   .type = STT_MOD,
                                   .current_index = 0,
                                   .parent =
                                       &head->node.decl->expression->node
                                            .module_definition->symbol_table};
        } else if (stmt->node.decl->expression->type == EXPR_STRUCT_DEF) {
          stmt->node.decl->expression->node.struct_def->symbol_table =
              (struct SymbolTable){.head = NULL,
                                   .type = STT_STRUCT,
                                   .current_index = 0,
                                   .parent =
                                       &head->node.decl->expression->node
                                            .module_definition->symbol_table};
        }

        struct DataType *inferred_type =
            infer_type(annotator, stmt->node.decl->expression);

        printf("inferred type: ");
        print_data_type(inferred_type);
        puts("");

        insert_symbol_entry_in(
            annotator,
            &head->node.decl->expression->node.module_definition->symbol_table,
            (struct SymbolTableEntry){
                .symbol = stmt->node.decl->symbol,
                .type = inferred_type,
                .llvm_value = NULL,
                .llvm_structure_type = NULL,
                .next = NULL,
                .symbol_location = SL_LOCAL,
                .index = 0,
            });
        stmt->node.decl->data_type = inferred_type;


        queue_push(annotator->allocator, queue, stmt);
        stmt = stmt->next;
      }
    }
  }
  puts("Finished queue.");
  ;
}
