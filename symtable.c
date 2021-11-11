#include "symtable.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ast.h"


sym_node* sym_define(sym_node** table, char* symbol, ast_node* value){

	// If no exsisting table skip looking in the table.
	if (*table){
	// Look for symbol in table
		sym_node* node = *table;
		while( node->next ){
			// If in the table update the value
			if ( strcmp(symbol, node->symbol) ){
				node->value->ref_count--;
				ast_free(node->value); // Try and free up the now orphied ast
				node->value = value;
				return *table;
			}
			node = node->next;
		}
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

ast_node* sym_lookup(sym_node** table, char* symbol){

	// Table must not be null
	assert(*table);

	sym_node* node = *table;
	do{
		if (! strcmp(symbol, node->symbol))
			return node->value;
		else
			node = node->next;
	}while( node->next );
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
