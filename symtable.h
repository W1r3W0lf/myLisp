#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "ast.h"

typedef struct sym_node{
	char* symbol;
	ast_node* value;
	struct sym_node *next;
} sym_node;

sym_node* sym_define(sym_node* table, char* symbol, ast_node* value);

ast_node* sym_lookup(sym_node* table, char* symbol);

int sym_free(sym_node* table);

#endif