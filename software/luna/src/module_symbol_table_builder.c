#include "module_symbol_table_builder.h"
#include "annotator.h"
#include <assert.h>
#include <stdio.h>

void mstb_visit_module(struct Annotator *annotator,
                       struct ModuleStatementNode *root) {
  assert(annotator != NULL);
  assert(root != NULL);

  printf("[mstb_visit_module] Found mod decl: '%s' of type: '",
         root->node.decl->symbol.data);
  print_data_type(root->node.decl->data_type);
  printf("'\n");
}
