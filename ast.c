#include "ast.h"

#include <stdlib.h>
#include <assert.h>

// The number of children a node should have based on it's type
const int children_num[] = {
	0, // number
	0, // string
	0, // symbol
	2, // definition
	3, // conditinal
	2, // cons_cell
	1  // quote
};


ast_node* ast_new_node(ast_node_type type){
	ast_node *new_node = malloc(sizeof(ast_node));
	new_node->type = type;

	if (children_num[type] > 0)
		new_node->children = malloc(sizeof(ast_node*) * children_num[type]);
	else
		new_node->children = NULL;

	new_node->child_count = 0;

	new_node->ref_count = 0;

	return new_node;
}


int ast_add_child(ast_node *parent, ast_node *child){

	// There must be room for another child
	assert(parent->child_count < children_num[parent->child_count]);

	parent->children[++parent->child_count] = child;
	child->ref_count++;

	return 0;
}

int ast_free(ast_node *root){

	if (root->ref_count > 0)
		return 1;

	for(int i = 0 ; i < root->child_count ; i++){
		root->children[i]->ref_count--; // Decrament the child's refrince count.
		ast_free(root->children[i]); // Free the child
	}

	// If this node had any children free it's refrinces to them.
	if (root->child_count > 0)
		free(root->children);

	// Free this nodes memory
	free(root);

	return 0;
}
