#include "ast.h"
#include "symtable.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>


sym_node* sym_define(sym_node** table, char* symbol, ast_node* value){

	// The symbol table must have a pointer to return the results to
	assert(table != NULL);

	// If no exsisting table skip looking in the table.
	if (*table){
	// Look for symbol in table
		sym_node* node = *table;
		do{
			// If in the table update the value
			if (! strcmp(symbol, node->symbol) ){
				node->value->ref_count--;
				ast_free(node->value); // Try and free up the now orphied ast
				node->value = value;
				return *table;
			}
			node = node->next;
		}while( node );
	}

	// If not in table create new node.
	sym_node *new_node = malloc(sizeof(sym_node));
	new_node->symbol = strdup(symbol);
	new_node->value = value;

	// Add the new node to the front of the list.
	new_node->next = *table;
	*table = new_node;


	return new_node;
}


/* Sym_tmp_define
 * This function temporarily defines vareables into the namespace without updaing there values.
 * It is used for defining opperands to functions.
 */
sym_node* sym_tmp_define(sym_node* table, char* symbol, struct ast_node* value){

	// The symbol table must have a pointer to return the results to
	assert(table != NULL);

	sym_node *new_node = malloc(sizeof(sym_node));
	new_node->symbol = strdup(symbol);
	new_node->value = value;

	// Add the new node to the front of the list.
	new_node->next = table;

	return new_node;
}

void sym_tmp_clean(sym_node** table, sym_node* tmp_table){

	sym_node* current_node = tmp_table;
	sym_node* next_node;

	while (current_node != *table){
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}

}


ast_node* sym_lookup(sym_node** table, char* symbol){

	// Table must not be null
	if(! *table)
		return NULL;

	sym_node* node = *table;
	do {
		if (! strcmp(symbol, node->symbol))
			return node->value;
		node = node->next;
	}while( node );
	return NULL;
}

int sym_free(sym_node** table){

	// Table must not be null
	assert(table);

	sym_node* node = *table;
	do {
		free(node->symbol);
		ast_free(node->value);
		node = node->next;
	} while( node );

	return 0;
}
