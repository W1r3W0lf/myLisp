#include "interp.h"

#include <stdio.h>
#include <assert.h>

#include "ast.h"
#include "symtable.h"


ast_node* eval(sym_node** active_symtable, ast_node* root){
	ast_node* return_node;

	assert(root != NULL);

	switch( root->type ){
		case number:
			printf("%d\n", root->value.number);
			break;
		case string:
			printf("String\n");
			break;
		case symbol:
			printf("Symbol\n");
			ast_node* found_symbol = sym_lookup(active_symtable, root->value.string);

			if (found_symbol){
				printf("Evaluating %s\n",root->value.string);
				eval(active_symtable,found_symbol);
			} else {
				printf("ERROR definition not found\n");
			}

			break;
		case quote:
			return_node = root;
			break;
		case cons_cell:
			printf("Cons Cell\n");
			break;
		case function:
			printf("Evaluating function\n");
			break;
		case function_pointer:
			return_node = root->value.function(active_symtable, root->children[0]);
			break;
		case definition:

			printf("Defining %s\n", root->children[0]->value.symbol);

			*active_symtable = sym_define(active_symtable, root->children[0]->value.symbol, root->children[1]);
			// Add symbol to the symbol table
			

			break;
		case expression:

			break;
		case conditinal:

			// Loop though conditions until one is true
			// Evauate the True path

			break;
		default:
			fprintf(stderr, "ERROR UNKNOWN AST_NODE type\n");
			break; //No need for a break here- it's just a place holer.
	}

	return return_node;
}


ast_node* apply(sym_node** symtable, ast_node* root){
	ast_node* return_node;

	if (root->type == function_pointer){
	}

	return return_node;
}


// Input / Output

ast_node* print(sym_node** symtable, ast_node* root){
	ast_node* return_node;

	switch ( root->type ) {
		case number:
			printf("%d\n",root->value.number);
			break;
		case string:
			printf("%s\n",root->value.string);
			break;
		default :
			printf("Printing Error\n");
	}

	return return_node;
}

// Arithmetic functions

ast_node* add(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	if ( root->child_count == 0 )
		fprintf(stderr, "ERROR: Add has no children\n");

	assert( root->child_count > 0 );

	ast_node* active_node;
	int sum = 0;
	active_node = root->children[0];
	while (active_node->child_count > 0){

		// I'm not sure if I should be doing this here.
		// Should I assume that all cildren of this node have been evaulated?
		// I think so... I'll come back to this later.
		switch ( active_node->type ) {
			case number:
				sum += active_node->value.number;
				break;
			case symbol:
				break;
			default:
				break;
		}

	}

	return return_node;
}

ast_node* subtract(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);


	return return_node;
}

ast_node* multiply(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	return return_node;
}

ast_node* devide(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	return return_node;
}


// Logical Operators

ast_node* and(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	return return_node;
}

ast_node* or(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	return return_node;
}

ast_node* not(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	return return_node;
}


sym_node* default_symtable(){
	sym_node* symtable;

	static int defualt_function_count = 10;

	char default_names[][10] = { "eval", "apply", "print", "+", "-", "*", "/", "and", "or", "not" };
	ast_node* (*default_functions[])(sym_node**, ast_node*) = {eval, apply, print, add, subtract, multiply, devide, and, or, not};



	ast_node *new_function_pointer;

	for (int i = 0 ; i < defualt_function_count ; i++){
		new_function_pointer = ast_new_node(function_pointer);
		new_function_pointer->value.function = default_functions[i];
	}


	return symtable;
}
