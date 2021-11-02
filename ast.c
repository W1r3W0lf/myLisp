#include "ast.h"

#include <stdlib.h>
#include <assert.h>

// The number of children a node should have based on it's type
const int children_num[] = {
	0, // number 1
	0, // string "string"
	0, // symbol a
	2, // function (lambda (a b) (+ a b)) -> (a b) (+ a b)
	1, // function_pointer (+ 1 2) -> (1 2) plus would be inplied
	1, // quote   '(a b c) -> (a b c)
	2, // cons_cell (a . b) -> (a . b)
	3, // definition (define (a b c) (+ b c) ) -> a (b c) (+ b c)
	2, // expression (+ 1 2) -> + (1 2)
	1, // conditinal (cond ((eq 1 2) 2) ((eq 1 1) 1) ) -> (((eq 1 2) 2) ((eq 1 1) 1))
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
	assert(parent->child_count <= children_num[parent->type]);

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
