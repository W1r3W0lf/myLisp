#include "interp.h"

#include <stdio.h>
#include <assert.h>

#include "ast.h"
#include "symtable.h"


ast_node* eval(sym_node** active_symtable, ast_node* root){

	assert(root != NULL);

	switch( root->type ){
		case number:
			return root;
		case string:
			return root;
		case symbol:
			return sym_lookup(active_symtable, root->value.string);
		case quote:
			return root->children[0];
		case cons_cell:
			printf("");

			if (root->child_count == 0)
				return root;

			ast_node* new_root;

			// If there is a symbol in place of a function, turn it into a function
			if (root->children[0]->type == symbol)
				new_root = eval(active_symtable,root->children[0]);
			else
				new_root = root->children[0];

			ast_node* result;
			// Not quite
			// Add the rest of the list as an opperand to the function
			if (new_root->type == function_pointer){

				ast_add_child(new_root, root->children[1]);

				result = eval(active_symtable, new_root);

				// Remove the children
				ast_remove_child(new_root);

				return result;
			} else if (new_root->type == function) {

				sym_node* tmp_table = *active_symtable;

				ast_node* op_name = new_root->children[0];
				ast_node* op_value = root->children[1];

				// Match up opperand names and values
				do{

					// Make a temporary symbol table with those pairs
					tmp_table = sym_tmp_define(tmp_table, op_name->children[0]->value.symbol, op_value->children[0]);

					op_name = op_name->children[1];
					op_value = op_value->children[1];

				} while( op_name->child_count > 0 && op_value->child_count > 0  );

				if (  op_name->child_count > 0 || op_value->child_count > 0 )
					printf("ERROR Opperand miss match\n");
				
				// Evalute the function's AST with the temporary symbol table
				
				result = eval(&tmp_table, new_root->children[1]);

				// Clean up the temporary symbol table
			
				sym_tmp_clean(active_symtable, tmp_table);

				return result;
			}


			printf("ERROR non-runable datatype encounterd\n");

			break;
		case function:
			// Returns a procedure

			// Match up opperand names and values
			// Make a temporary symbol table with those pairs
			// Evalute the function's AST with the temporary symbol table
			// Clean up the temporary symbol table

			break;
		case function_pointer:
			// Returns a procedure
			
			// Remove this and have it just return the procedure
			return root->value.function(active_symtable, root->children[0]);
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
		case cons_cell:
			if (root->child_count == 2){
				print(symtable, root->children[0]);
				print(symtable, root->children[1]);
			}
			break;
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

	ast_node* active_cons;
	ast_node* active_node;
	int sum = 0;
	active_cons = root;
	while (active_cons->child_count > 0){

		active_node = active_cons->children[0];
		switch ( active_node->type ) {
			case number:
				sum += active_node->value.number;
				break;
			case symbol:
				active_node = eval(symtable, active_node);
				if (active_node->type == number)
					sum += active_node->value.number;
				else
					printf("ERROR symbol returned a non-number");
				break;
			default:
				printf("ERROR a non-number has made it into add\n");
				break;
		}
		active_cons = active_cons->children[1];

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
