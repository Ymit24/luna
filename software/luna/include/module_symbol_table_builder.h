#ifndef MODULE_SYMBOL_TABLE_BUILDER_H
#define MODULE_SYMBOL_TABLE_BUILDER_H

#include "annotator.h"
#include "ast.h"

void mstb_visit_module(struct Annotator *annotator,
                       struct ModuleStatementNode *root);

#endif
