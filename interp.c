#include "interp.h"

#include "ast.h"
#include "symtable.h"

ast_node* eval(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	switch( root->type ){
		case number:
		case string:
		case symbol:
		case quote:
			return_node = root;
			break;
		case cons_cell:
			break;
		case function:
			break;
		case function_pointer:
			return_node = root->value.function(symtable, root->children[0]);
			break;
		case definition:

			// Add symbol to the symbol table

			break;
		case expression:

			break;
		case conditinal:

			// Loop though conditions until one is true
			// Evauate the True path

			break;
		default:
			break; //No need for a break here- it's just a place holer.
	}

	return return_node;
}


ast_node* apply(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	if (root->type == function_pointer){
	}

	return return_node;
}


// Input Output

ast_node* print(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}

// Arithmetic functions

ast_node* add(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}

ast_node* subtract(sym_node* symtable, ast_node* root){
	ast_node* return_node;


	return return_node;
}

ast_node* multiply(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}

ast_node* devide(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}


// Logical Operators

ast_node* and(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}

ast_node* or(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}

ast_node* not(sym_node* symtable, ast_node* root){
	ast_node* return_node;

	return return_node;
}


sym_node* default_symtable(){
	sym_node* symtable;

	static int defualt_function_count = 10;

	char default_names[][10] = { "eval", "apply", "print", "+", "-", "*", "/", "and", "or", "not" };
	ast_node* (*default_functions[])(sym_node*, ast_node*) = {eval, apply, print, add, subtract, multiply, devide, and, or, not};



	ast_node *new_function_pointer;

	for (int i = 0 ; i < defualt_function_count ; i++){
		new_function_pointer = ast_new_node(function_pointer);
		new_function_pointer->value.function = default_functions[i];
	}


	return symtable;
}
