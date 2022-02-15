#include "interp.h"

#include <stdio.h>
#include <assert.h>

#include "ast.h"
#include "symtable.h"

// I hate how I keep on having to add both a sym_node** and a ast_node* to every function
// I'm going to make a struct that will combine them both.
//
// Before I do this, I should relise namespaces.
// That's the whole point why I have these functions take in a symbol table
// The dumb way would be to just refrence a global symbol table
// But that would prevent concurrent programming, and remove namespaces from the table.
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

			// This is a place holder
			// I don't currently have any truthy or falsey values
			// So for now I'm using 0 as my falsey value
			// And any other intiger as my truthy values
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


// Supporting functions

ast_node* evaluate_opperands(sym_node** symtable, ast_node* root){

	// Make a new list for the results to be placed into
	ast_node* active_node = root;
	// Holds the list that will be returned
	ast_node* final_result = ast_new_node(cons_cell);
	// Points to where the next result should be placed.
	ast_node* result_node = final_result;
	// A place holder so the next node can be added to the list.
	ast_node* last_node = final_result;

	// Itterate over the elements into the input list
	while( active_node->child_count > 0 ){
		// Break if it's an impropper list
		assert(active_node->type == cons_cell);

		if (active_node != root ){
			// Shimmy in another node to add the result into
			last_node = result_node;
			result_node = ast_new_node(cons_cell);
			ast_add_child(last_node, result_node);
		}

		// Evaluate elemtns and place results into results list
		ast_add_child(result_node, eval(symtable, active_node->children[0]));

		// Move on to the next cons cell
		active_node = active_node->children[1];
	}

	// Place a nil at the end of the list, to make it propper
	ast_add_child(result_node, ast_new_node(cons_cell));

	// Free the input list

	// Return the results list
	return final_result;
}


// Input / Output

ast_node* print(sym_node** symtable, ast_node* root){
	ast_node* return_node;

	ast_node* active_cons;

	assert(root != NULL);

	// TODO How should I add the final \n ?
	// I could have a handler function that calls the main print function and prints a \n
	switch ( root->type ) {
		case cons_cell:

			active_cons = root;

			printf("(");

			// TODO What if a cons cell only has one child?
			while (active_cons->child_count > 1) {
				print(symtable, active_cons->children[0]);
				printf(" ");
				active_cons = active_cons->children[1];
			}

			printf(")");

			break;
		case number:
			printf("%d",root->value.number);
			break;
		case symbol:
		case string:
			printf("%s",root->value.string);
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
	
	// Not quite, I don't want to evaluate the next value across the rest of the opperands.
	//ast_node* result = eval(symtable, root);

	ast_node* opperands = evaluate_opperands(symtable, root);

	// (func 1 a (+ 1 1) '(a b c)) where a = 7
	// should get
	// (func 1 7 2 (a b c))
	//
	// This should apply to every function.
	// So my solution here will be applied to all other functions.
	// Don't rely on the fact that car only expects one opperand.
	
	// (map eval '(x y z))
	// should do it, hummm
	// But if I use map here, I can't use car in map.
	//
	// I'll try writting map first.


	// Check to make sure that it's unwrapping a cons cell
	if (root->type != cons_cell)
		fprintf(stderr, "ERROR, you can only use car on a cons cell\n");
	assert(opperands->children[0]->type == cons_cell);
	// Check to see if the cons cell has at least 1 item in it
	assert(opperands->children[0]->child_count > 0);

	// Retuen the first item in the cons cell
	return opperands->children[0]->children[0];
}

ast_node* cdr(sym_node** symtable, ast_node* root){
	ast_node* opperands = evaluate_opperands(symtable, root);
	if (root->type != cons_cell)
		fprintf(stderr, "ERROR, you can only use cdr on a cons cell\n");
	assert(root->type == cons_cell);
	assert(root->child_count > 1);
	assert(opperands->children[0]->type == cons_cell);
	// Check to see if the cons cell has at least 1 item in it
	assert(opperands->children[0]->child_count > 0);

	// Retuen the first item in the cons cell
	return opperands->children[0]->children[1];
}

ast_node* cons(sym_node** symtable, ast_node* root){

	ast_node* return_node = ast_new_node(cons_cell);

	assert(root != NULL);

	// Evaluate the rest of the S-expression
	ast_node* opperands = evaluate_opperands(symtable, root);

	// TODO Check to see if there are exactly two opperands

	// Place the two opperands into the cons cell
	ast_add_child(return_node, opperands->children[0]);
	ast_add_child(return_node, opperands->children[1]);

	// Return the cons cell
	return return_node;
}

ast_node* map(sym_node** symtable, ast_node* root){
	ast_node* return_node;
	return_node = ast_new_node(number);

	assert(root != NULL);

	// Both of these need to be evaluated.
	ast_node* function = root->children[0];
	ast_node* opperand = root->children[1];

	while (opperand->child_count > 0){

	}

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
