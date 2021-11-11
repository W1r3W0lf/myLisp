#ifndef INTERP_H
#define INTERP_H

#include "ast.h"
#include "symtable.h"

ast_node* eval(sym_node** symtable, ast_node* root);

ast_node* apply(sym_node* symtable, ast_node* root);

sym_node* default_symtable();

#endif
