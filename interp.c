#include "interp.h"

#include <stdio.h>
#include <assert.h>

#include "ast.h"
#include "symtable.h"


ast_node* eval(sym_node** active_symtable, ast_node* root){

	assert(root != NULL);

	switch( root->type ){
		case number:
			printf("%d\n", root->value.number);
			return root;
		case string:
			printf("%s\n",root->value.string);
			return root;
		case symbol:
			return sym_lookup(active_symtable, root->value.string);
		case quote:
			return root->children[0];
		case cons_cell:
			printf("");


			// Convert symbol into function with eval
			// Or evauluate user function over input data
			ast_node* new_root = root->children[0];

			// Turn the symbol into a function or function pointer
			new_root = eval(active_symtable,root->children[0]);

			// Not quite
			// Add the rest of the list as an opperand to the function
			ast_add_child(new_root, root->children[1]);

			ast_node* result = eval(active_symtable, new_root);

			eval(active_symtable,result);
			printf("RESULT %d\n", result->value.number);
		
			return result;
		case function:
			// Returns a procedure
			break;
		case function_pointer:
			// Returns a procedure
			
			// Remove this and have it just return the procedure
			return root->value.function(active_symtable, root->children[0]);
			return root;
		case definition:

			*active_symtable = sym_define(active_symtable, root->children[0]->value.symbol, root->children[1]);
			// Add symbol to the symbol table
	
			// Equal to return nil
			return ast_new_node(cons_cell);
		case expression:
			// I don't know if I need/want this.
			// I'll leave it here for now, in case I decide to go down this route again.

			break;
		case conditinal:

			// Loop though conditions until one is true
			// Evauate the True path

			break;
		default:
			fprintf(stderr, "ERROR UNKNOWN AST_NODE type\n");
			break; //No need for a break here- it's just a place holer.
	}

	// This line of code should never run after eval is fully working
	return root;
}


ast_node* apply(sym_node** symtable, ast_node* root){

	assert(root != NULL);

	ast_node* return_node;

	if (root->type == function_pointer){
	}

	return return_node;
}


// Input / Output

ast_node* print(sym_node** symtable, ast_node* root){
	ast_node* return_node;

	assert(root != NULL);

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

	assert(root != NULL);

	if ( root->child_count == 0 )
		fprintf(stderr, "ERROR: Add has no children\n");

	assert( root->child_count > 0 );

	ast_node* active_node;
	int sum = 0;
	active_node = root;
	while (active_node->child_count > 0){

		// I'm not sure if I should be doing this here.
		// Should I assume that all cildren of this node have been evaulated?
		// I think so... I'll come back to this later.
		switch ( active_node->children[0]->type ) {
			case number:
				sum += active_node->children[0]->value.number;
				break;
			case symbol:
				break;
			default:
				break;
		}
		active_node = active_node->children[1];

	}

	return_node->value.number = sum;

	return return_node;
}

ast_node* subtract(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}

ast_node* multiply(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}

ast_node* devide(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}


// Logical Operators

ast_node* and(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}

ast_node* or(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}

ast_node* not(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}


sym_node* default_symtable(){
	sym_node* symtable = NULL;

	static int defualt_function_count = 10;

	char default_names[][10] = { "eval", "apply", "print", "+", "-", "*", "/", "and", "or", "not" };
	ast_node* (*default_functions[])(sym_node**, ast_node*) = {eval, apply, print, add, subtract, multiply, devide, and, or, not};

	ast_node *new_function_pointer;
	char* new_function_name;

	for (int i = 0 ; i < defualt_function_count ; i++){
		new_function_pointer = ast_new_node(function_pointer);
		new_function_pointer->value.function = default_functions[i];
		new_function_name = default_names[i];

		symtable = sym_define(&symtable,new_function_name, new_function_pointer);
	}


	return symtable;
}
