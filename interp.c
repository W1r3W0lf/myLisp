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
		case definition:

			// Add symbol to the symbol table

			break;
		case conditinal:

			// Loop though conditions until one is true
			// Evauate the True path

			break;
		case cons_cell:

			// Apply the function across the rest of the cons cells.

			break;
		default:
			break; //No need for a break here- it's just a place holer.
	}

	return return_node;
}
