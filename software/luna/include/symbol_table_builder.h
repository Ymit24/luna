#ifndef SYMBOL_TABLE_BUILDER_H
#define SYMBOL_TABLE_BUILDER_H

#include "annotator.h"

void sym_tbl_build(struct Annotator *annotator,
                   struct ModuleStatementNode *root_module);

#endif
