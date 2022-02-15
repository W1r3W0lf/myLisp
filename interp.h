#ifndef INTERP_H
#define INTERP_H

#include "ast.h"
#include "symtable.h"


// This is the idea of what a combined execution context should look like.
typedef struct {
	sym_node** symtable;
	ast_node* root;
} context;

// I am unsure if I will need supporting functions for the execution context
// But here is where they would go.

ast_node* eval(sym_node** symtable, ast_node* root);

ast_node* apply(sym_node** symtable, ast_node* root);

ast_node* print(sym_node** symtable, ast_node* root);

sym_node* default_symtable();

#endif
