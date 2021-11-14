%{
	#include <stdio.h>
	#include <string.h>

	#include "ast.h"

	extern sym_node* global_symboltable;
	extern ast_node* ast;

	extern int yylex();
	extern int yyparse();
	void yyerror(const char *s);
%}

%union{
	char* symbol;
	int int_number;
	char* string;
	struct ast_node* ast;
}

%token <int_number> NUMBER
%token <symbol> SYMBOL
%token <string> STRING
%type <ast> datatype
%type <ast> cons
%type <ast> list
%type <ast> definition

%token DEFINE COND NIL LAMBDA QUOTE

%%

	statement:
		definition
	|	condition
	|	datatype {
		ast = $1;
	}
	|	cons {
		ast = $1;
	}
	;

	definition:
		'(' DEFINE SYMBOL datatype ')' {
		// Define the new parent node
		ast_node* node = ast_new_node(definition);

		// Create the symbol table
		ast_node* symbol_node = ast_new_node(symbol);
		symbol_node->value.symbol = strdup($3);

		// Add the children to the parent node
		ast_add_child(node, symbol_node);
		ast_add_child(node, $4);

		$$ = node;
		
		// return to the ast
		ast = node;

		printf("%s has been defined\n", $3);
		}
	;


	
	condition:
		'(' COND list')' {
			ast_node* node = ast_new_node(cons_cell);
			ast_add_child(node, $3);

		}
	;

	cons:
		'(' datatype '.' datatype ')' {
			ast_node* node = ast_new_node(cons_cell);
			ast_add_child(node, $2);
			ast_add_child(node, $4);
			printf("cons\n");
			$$ = node;
		}
	|	'(' list ')' {
			printf("list\n");
			$$ = $2;
		}
	;

	list:
		datatype {
			$$ = $1;
		}
	|	datatype list {
		ast_node* node = ast_new_node(cons_cell);
		ast_add_child(node, $1);
		ast_add_child(node, $2);
		$$ = node;
	}
	|	cons
	|	cons list
	;

	datatype:
		NUMBER {
			printf("%d\n",$1);
			ast_node* node = ast_new_node(number);
			node->value.number = $1;
			$$ = node;
		}
	|
		STRING {
			printf("%s\n", $1);
			ast_node* node = ast_new_node(string);
			node->value.string = strdup($1);
			$$ = node;
			free($1);
		}
	|
		SYMBOL {
			ast_node* node = ast_new_node(symbol);
			node->value.symbol = strdup($1);
			printf("Symbol %s\n", node->value.symbol);
			$$ = node;
			free($1);
		}
	|	NIL {
			ast_node* node = ast_new_node(nil);
			$$ = node;
	}
	|	'(' LAMBDA '(' list ')' '(' list ')' ')' {
			ast_node* node = ast_new_node(definition);
			printf("Function\n");
			ast_add_child(node, $4);
			ast_add_child(node, $7);
			$$ = node;
		}
	;

%%

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
