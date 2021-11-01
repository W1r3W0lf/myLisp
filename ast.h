#ifndef AST_H
#define AST_H

typedef enum ast_node_type {
	number,
	string,
	symbol,
	definition,
	conditinal,
	cons_cell,
	quote
} ast_node_type;

typedef union ast_value{
	char* string;
	int number;
} ast_value;

typedef struct ast_node{
	struct ast_node **children; // An array of pointers to this nodes children
	ast_value value;
	ast_node_type type;
	int child_count; // How many children does this node currently have
} ast_node;

// A nil value is reprented as a NULL pointer.

ast_node* ast_new_node(ast_node_type type);

int ast_add_child(ast_node *parent, ast_node *child);

int ast_free(ast_node *root);


#endif
