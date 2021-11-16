#ifndef AST_H
#define AST_H

#include "symtable.h"
#include <stdbool.h>


typedef enum ast_node_type {
	number,
	string,
	symbol,
	function, // A function written in myLisp
	function_pointer, // A function that's built into the interpriter
	quote,
	cons_cell,
	definition,
	expression,
	conditinal
} ast_node_type;

struct ast_node;

typedef union ast_value{
	struct ast_node* (*function)( struct sym_node**, struct ast_node*);
	char* symbol;
	char* string;
	int number;
} ast_value;

typedef struct ast_node{
	struct ast_node **children; // An array of pointers to this nodes children
	ast_value value;
	ast_node_type type;
	int child_count; // How many children does this node currently have
	int ref_count; // How many times has this node been refrinced, if it is grater then 1 at the time of deletion it will not be deleted.
} ast_node;

ast_node* ast_new_node(ast_node_type type);

int ast_add_child(ast_node *parent, ast_node *child);

int ast_remove_child(ast_node *parent);

int ast_free(ast_node *root);

void ast_print(ast_node *root);

#endif
