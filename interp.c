#include "interp.h"

#include <stdio.h>
#include <assert.h>

#include "ast.h"
#include "symtable.h"


ast_node* eval(sym_node** active_symtable, ast_node* root){

	assert(root != NULL);

	switch( root->type ){
		case number:
			// An intiger
			return root;
		case string:
			// A sequence of characters
			return root;
		case symbol:
			// A string that can refer to some value
			return sym_lookup(active_symtable, root->value.string);
		case quote:
			// Returns the quoted symbol instead of the symbol's value
			return root->children[0];
		case cons_cell:
			printf("");

			// () -> () aka the empty list is nil
			// And self evaluates
			if (root->child_count == 0)
				return root;

			ast_node* new_root;

			// Evaluate symbol
			if (root->children[0]->type == symbol)
				new_root = eval(active_symtable,root->children[0]);
			else
				new_root = root->children[0];

			ast_node* result;
			// Not quite
			// Add the rest of the list as an opperand to the function
			if (new_root->type == function_pointer){

				ast_node* (*func)(sym_node**, ast_node*) = new_root->value.function;

				new_root = ast_new_node(function_pointer);
				new_root->value.function = func;

				ast_add_child(new_root, root->children[1]);

				result = eval(active_symtable, new_root);
				ast_free(new_root);

				// Remove the children
				//ast_remove_child(new_root);

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
					fprintf(stderr, "ERROR Opperand miss match\n");

				// Evalute the function's AST with the temporary symbol table

				result = eval(&tmp_table, new_root->children[1]);

				// Clean up the temporary symbol table

				sym_tmp_clean(active_symtable, tmp_table);

				return result;
			}

			fprintf(stderr, "ERROR non-runable datatype encounterd\n");

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

			printf("");

			ast_node* condition = eval(active_symtable, root->children[0]);

			if (condition->value.number)
				return eval(active_symtable, root->children[1]);
			else
				return eval(active_symtable, root->children[2]);

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
		case symbol:
		case string:
			printf("%s\n",root->value.string);
			break;
		default :
			fprintf(stderr, "Printing Error\n");
	}

	return return_node;
}

// Arithmetic functions

// TODO rename this to it's actual name
ast_node* arithmetic(sym_node** symtable, ast_node* root, int( a_function )(int, int) ){
	ast_node* return_node = ast_new_node(number);

	assert(root != NULL);

	if ( root->child_count == 0 )
		fprintf(stderr, "ERROR: an arithmetic function has no children\n");

	assert( root->child_count > 0 );

	ast_node* active_node = root;
	int result = 0;

	int primed = false;

	while (active_node->child_count > 0){
		switch ( active_node->children[0]->type ){
			case number:

				if (primed)
					result = a_function(result, active_node->children[0]->value.number);
				else{
					result = active_node->children[0]->value.number;
					primed = true;
				}
				active_node = active_node->children[1];
				break;
			case cons_cell:
			case symbol:
				active_node->children[0] = eval(symtable, active_node->children[0]);
				break;
			default:
				fprintf(stderr, "ERROR, an unexpected ast node has entered an arithmetic function\n");
		}
	}

	return_node->value.number = result;

	return return_node;
}

int a_add(int a, int b){
	return a + b;
}

ast_node* add(sym_node** symtable, ast_node* root){
	return arithmetic(symtable, root, a_add);
}

int a_subtract(int a, int b){
	return a - b;
}
ast_node* subtract(sym_node** symtable, ast_node* root){
	return arithmetic(symtable, root, a_subtract);
}

int a_multiply(int a, int b){
	return a * b;
}
ast_node* multiply(sym_node** symtable, ast_node* root){
	return arithmetic(symtable, root, a_multiply);
}

int a_devide(int a, int b){
	if (b == 0)
		fprintf(stderr, "DEVIDE BY ZERO ERROR\n");
	assert(b != 0);
	return a / b;
}
ast_node* devide(sym_node** symtable, ast_node* root){
	return arithmetic(symtable, root, a_devide);
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

// Data structure opperators

ast_node* car(sym_node** symtable, ast_node* root){

	// Evaluate the rest of the S-expression

	// Check to make sure that it's unwrapping a cons cell
	// Checl to see if the cons cell has at least 1 item in it

	// Retuen the first item in the cons cell

	if (root->type != cons_cell)
		fprintf(stderr, "ERROR, you can only use car on a cons cell\n");
	assert(root->type == cons_cell);
	assert(root->child_count > 0);

	// Double indexing into children is because opperands are lists
	// The first is to index into opperands the second is going into the given cons cell
	return root->children[0]->children[0];
}

ast_node* cdr(sym_node** symtable, ast_node* root){
	if (root->type != cons_cell)
		fprintf(stderr, "ERROR, you can only use cdr on a cons cell\n");
	assert(root->type == cons_cell);
	assert(root->child_count > 1);
	return root->children[0]->children[1];
}

ast_node* cons(sym_node** symtable, ast_node* root){

	ast_node* return_node;
	return_node = ast_new_node(cons_cell);

	assert(root != NULL);

	// Evaluate the rest of the S-expression

	// Check to see if there are exactly two opperands

	// Place the two opperands into the cons cell

	// Return the cons cell
	return return_node;
}

ast_node* map(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	return return_node;
}

sym_node* default_symtable(){
	sym_node* symtable = NULL;

	static int defualt_function_count = 14;

	char default_names[][10] = { "eval", "apply", "print", "+", "-", "*", "/", "and", "or", "not", "car", "cdr", "cons", "map" };
	ast_node* (*default_functions[])(sym_node**, ast_node*) = {eval, apply, print, add, subtract, multiply, devide, and, or, not, car, cdr, cons, map};

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
