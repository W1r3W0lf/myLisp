#ifndef INTERP_H
#define INTERP_H

#include "ast.h"
#include "symtable.h"

ast_node* eval(sym_node* symtable, ast_node* root);

#endif
