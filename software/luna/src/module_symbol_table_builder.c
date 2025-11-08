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

  root->node.decl->data_type =
      infer_type(annotator, root->node.decl->expression);

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
        assert(inferred_type != NULL);

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
}

void mstb_resolve_types(struct Annotator *annotator, struct DataType *type) {
  switch (type->kind) {
  case DTK_FUNCTION: {
    mstb_resolve_types(annotator, type->value.function.return_type);
    struct FunctionArgumentNode *argument = type->value.function.arguments;
    while (argument != NULL) {
      mstb_resolve_types(annotator, argument->data_type);
      argument = argument->next;
    }
    break;
  }
  case DTK_RESOLVABLE: {
    if (type->value.resolvable.resolved_type == NULL) {
      break;
    }
    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
        type->value.resolvable.scoped_symbol, annotator->current_symbol_table);
    assert(entry != NULL);
    assert(entry->type != NULL);

    puts("Resolved type:");
    print_data_type(entry->type);
    assert(0);

    type->value.resolvable.resolved_type = entry->type;
    break;
  }
  case DTK_POINTER:
    mstb_resolve_types(annotator, type->value.pointer_inner);
    break;
  case DTK_ARRAY:
    mstb_resolve_types(annotator, type->value.array.element_type);
    break;
  case DTK_STRUCTURE: {
    puts("mstb_resolve_types: DTK_STRUCTURE. will remove this soon");
    struct SymbolTableEntry *entry = lookup_scoped_symbol_in(
        type->value.structure.name, annotator->current_symbol_table);

    assert(entry != NULL);
    assert(entry->type != NULL);
    assert(entry->type->kind == DTK_STRUCTURE_DEF);

    printf("For (%s), resolved structure type:\n", entry->symbol.data);
    print_data_type(entry->type);
    puts("");

    type->value.structure.definition =
        entry->type->value.structure_definition.definition;

    assert(entry->type->value.structure_definition.definition != NULL);

    printf("mstb_resolve_types: DTK_STRUCTURE. resolved structure type:\n");
    print_data_type(type);
    puts("");

    assert(type->value.structure.definition != NULL);
    break;
  }
  case DTK_STRUCTURE_DEF: {
    struct StructFieldDefinitionNode *field =
        type->value.structure_definition.definition->fields;
    while (field != NULL) {
      mstb_resolve_types(annotator, field->type);
      field = field->next;
    }
    break;
  }
  case DTK_MODULE_DEF:
  case DTK_MODULE:
  case DTK_PRIMITIVE:
  case DTK_VOID:
    break;
  }
}

void mstb_infer_types(struct Annotator *annotator,
                      struct SymbolTable *symbol_table) {
  struct SymbolTableEntry *entry = symbol_table->head;

  while (entry != NULL) {
    printf("mstb_infer_types entry: %s\n", entry->symbol.data);
    assert(entry->type != NULL);

    struct SymbolTable *old_symbol_table = annotator->current_symbol_table;
    annotator->current_symbol_table = symbol_table;

    mstb_resolve_types(annotator, entry->type);

    printf("mstb_infer_types resolved type: ");
    print_data_type(entry->type);
    printf("\n");

    annotator->current_symbol_table = old_symbol_table;

    if (entry->type->kind == DTK_MODULE_DEF) {
      mstb_infer_types(annotator, &entry->type->value.module_definition
                                       ->module_definition->symbol_table);
    }
    entry = entry->next;
  }
}
