#include "ast.h"

// The number of children a node should have based on it's type
const int children_num[] = {
	0, // number
	0, // string
	0, // symbol
	2, // definition
	3, // conditinal
	2, // cons_cell
	1  // quote
};
