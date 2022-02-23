#include "ast.h"
#include "interp.h"
#include "symtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ast_node* evaluate_opperands(sym_node** symtable, ast_node* root);
int list_size(ast_node* root);

ast_node* error(char* error_symbol){
	ast_node* error_node = ast_new_node(symbol);
	error_node->value.symbol = strdup(error_symbol);
	return error_node;
}

ast_node* eval_function(sym_node** active_symtable, ast_node* root){
	// Returns a procedure
	ast_node* function_ast = root->children[0]->children[1];
	ast_node* opperands = root->children[1];
	ast_node* arguments = root->children[0]->children[0];

	// Ensure that arguments and opperands match

	if (list_size(arguments) != list_size(opperands)){
		fprintf(stderr, "ERROR opperator miss match\n");
		return error("opperator_ERROR");
	}

	sym_node* tmp_table = *active_symtable;

	ast_node* argument = arguments;
	ast_node* opperand = opperands;

	// Match up opperand names and values
	do{

		// Make a temporary symbol table with those pairs
		tmp_table = sym_tmp_define(tmp_table, argument->children[0]->value.symbol, opperand->children[0]);

		argument = argument->children[1];
		opperand = opperand->children[1];

	} while( argument->child_count > 0 && opperand->child_count > 0  );

	if (  argument->child_count > 0 || opperand->child_count > 0 )
		fprintf(stderr, "ERROR !! !! Operator miss match\n");

	// Evalute the function's AST with the temporary symbol table
	ast_node* result = eval(&tmp_table, function_ast);

	// Clean up the temporary symbol table
	sym_tmp_clean(active_symtable, tmp_table);

	return result;
}

ast_node* eval_function_pointer(sym_node** active_symtable, ast_node* root){
	ast_node* function_root = root->children[0];
	return function_root->value.function(active_symtable, root->children[1]);
}

ast_node* eval(sym_node** active_symtable, ast_node* root){

	assert(root != NULL);

	//repl_print(active_symtable, root);

	switch( root->type ){
		case symbol:
			// A string that can refer to some value
			return sym_lookup(active_symtable, root->value.string);
		case quote:
			// Returns the quoted symbol instead of the symbol's value
			return root->children[0];

		case cons_cell:
			// () -> () aka the empty list is nil
			// And self evaluates
			if (root->child_count == 0)
				return root;

			// If opperator is a symbol or function, evaluate it.
			ast_node* evaluated_list = evaluate_opperands(active_symtable, root);

			// TODO An If-Else will probabily work just fine here
			switch (evaluated_list->children[0]->type){
				case function:
					return eval_function(active_symtable, evaluated_list);
				case function_pointer:
					return eval_function_pointer(active_symtable, evaluated_list);
				default:
					fprintf(stderr, "ERROR, can't evaluate unsupported type.");
					return error("unknown_symbol_ERROR");
			}

			break;
		case definition:

			*active_symtable = sym_define(active_symtable, root->children[0]->value.symbol, root->children[1]);
			// Add symbol to the symbol table

			// Equal to return nil
			return ast_new_node(cons_cell);
		case conditinal:
			// Clang won't allow a definition at the beginning of a case
			// So I'm adding an assert to make it the second statement.
			assert(root->child_count > 1);

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
			// Default behavure is self evaluateion
			break;
	}

	return root;
}

ast_node* apply(sym_node** symtable, ast_node* root){

	assert(root != NULL);

	ast_node* return_node = ast_new_node(cons_cell);

	if (root->type == function_pointer){
	}

	return return_node;
}


// Supporting functions

// I've been noticing patterns like this in creating many functions
// Eventually I would like to re-write most of these functions in a subset of mylisp
// I don't know what that would look like right now, but I'll leave notes about my thougths
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
	while( active_node->child_count > 1 ){
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

	// TODO Free the input list

	// Return the results list
	return final_result;
}

int list_size(ast_node* root){
	int total = 0;
	ast_node* active_node = root;
	while (active_node->child_count > 1){
		total++;
		active_node = active_node->children[1];
	}
	return total;
}

// Input / Output


ast_node* print_runner(sym_node** symtable, ast_node* root){
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
				print_runner(symtable, active_cons->children[0]);
				if (active_cons->children[1]->child_count > 0)
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
		case quote:
			printf("'");
			print_runner(symtable, root->children[0]);
			break;
		case function:
			assert( 1==1 );
			int size = list_size(root->children[0]);
			printf("f%d#", size);
			break;
		case function_pointer:
			printf("fp#");
		default :
			fprintf(stderr, "Printing Error\n");
	}

	return root;
}

ast_node* print(sym_node** symtable, ast_node* root){
	ast_node* active_cons = root;

	while (active_cons->child_count > 1){
		print_runner(symtable, active_cons->children[0]);
		printf("\n");
		active_cons = active_cons->children[1];
	}
	return root;
}

ast_node* repl_print(sym_node** symtable, ast_node* root){
	print_runner(symtable, root);
	printf("\n");
	return root;
}

// Arithmetic functions

// Real name reduce
//
// TODO re-write this function to be used in the language itself
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
				exit(1);
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

	ast_node* opperands = evaluate_opperands(symtable, root);

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

	// Check to see if there are exactly two opperands
	int opperand_size = list_size(opperands);
	if (opperand_size != 2)
		fprintf(stderr, "ERROR cons can only take 2 arguments\n");
	assert(opperand_size == 2);

	// Place the two opperands into the cons cell
	ast_add_child(return_node, opperands->children[0]);
	ast_add_child(return_node, opperands->children[1]);

	// Return the cons cell
	return return_node;
}

// This rings of the evaluate_opperands function
// It's not suprising, my first idea for that function was to use map
//
// TODO Map is currently acting like reduce
ast_node* map(sym_node** symtable, ast_node* root){
	ast_node* result_list = ast_new_node(cons_cell);
	ast_node* last_result = result_list;
	ast_node* next_result;
	ast_node* result;
	ast_node* temp_ast;

	assert(root != NULL);

	ast_node* opperands = evaluate_opperands(symtable, root);

	// TODO Find a better way to handel errors.
	// Hand crafted errors for each function are bad.
	// And every error being fatial is worse. (Except in Erlang)
	int opperand_size = list_size(opperands);
	if (opperand_size != 2)
		fprintf(stderr, "ERROR map can only take 2 arguments\n");
	assert(opperand_size == 2);

	ast_node* function = opperands->children[0];
	ast_node* opperand = opperands->children[1];

	while (opperand->child_count > 1){

		// Combine the function and the argument to be run
		temp_ast = ast_new_node(cons_cell);
		ast_add_child(temp_ast, function);

		ast_add_child(temp_ast, opperand->children[0]->children[0]);


		result = eval(symtable, temp_ast);

		// Add result to result list
		ast_add_child(last_result, result);
		next_result = ast_new_node(cons_cell);
		ast_add_child(last_result, next_result);
		last_result = next_result;

		opperand = opperand->children[1];
	}

	return result_list;
}

ast_node* filter(sym_node** symtable, ast_node* root){
	return NULL;
}
/*
ast_node* reduce(sym_node** symtable, ast_node* root){
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
*/

ast_node* lisp_exit(sym_node** symtable, ast_node* root){
	exit(0);
	return NULL;
}


sym_node* default_symtable(){
	sym_node* symtable = NULL;

	// TODO Find a way to combine these three diffrent variables into something cohesive.
	static int defualt_function_count = 15;
	char default_names[][10] = { "eval", "apply", "print", "+", "-", "*", "/", "and", "or", "not", "car", "cdr", "cons", "map", "exit" };
	ast_node* (*default_functions[])(sym_node**, ast_node*) = {eval, apply, print, add, subtract, multiply, devide, and, or, not, car, cdr, cons, map, lisp_exit};

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
