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
%type <ast> list
%type <ast> list_content
%type <ast> definition
%type <ast> expression

%token DEFINE COND NIL LAMBDA QUOTE

%%

	statement:
		expression {
		ast = $1;
	}
	;

	expression:
		definition {
		ast = $1;
	}
	|	datatype {
		ast = $1;
	}
	|	QUOTE datatype {
		ast_node* node = ast_new_node(quote);
		ast_add_child(node, $2);
		$$ = node;
	}
	;

	definition:
		'(' DEFINE SYMBOL expression ')' {
		// Define the new parent node
		ast_node* node = ast_new_node(definition);

		// Create the symbol table
		ast_node* symbol_node = ast_new_node(symbol);
		symbol_node->value.symbol = strdup($3);

		// Add the children to the parent node
		ast_add_child(node, symbol_node);
		ast_add_child(node, $4);

		$$ = node;
		}
	;


	datatype:
		list {
		$$ = $1;
		}
	|	NUMBER {
			ast_node* node = ast_new_node(number);
			node->value.number = $1;
			$$ = node;
		}
	|
		STRING {
			ast_node* node = ast_new_node(string);
			node->value.string = strdup($1);
			$$ = node;
			free($1);
		}
	|
		SYMBOL {
			ast_node* node = ast_new_node(symbol);
			node->value.symbol = strdup($1);
			$$ = node;
			free($1);
		}
	|	NIL {
			ast_node* node = ast_new_node(nil);
			$$ = node;
	}
	|	'(' LAMBDA list list ')' {
			ast_node* node = ast_new_node(definition);
			ast_add_child(node, $3);
			ast_add_child(node, $4);
			$$ = node;
		}
	|	'(' COND list ')' {
			ast_node* node = ast_new_node(conditinal);
			ast_add_child(node, $3);
		}
	;

	list:
		'(' list_content ')' {
			$$ = $2;
		}
	;

	list_content:
		expression {
		ast_node* node = ast_new_node(cons_cell);
		ast_node* nil_node = ast_new_node(nil);
		ast_add_child(node, $1);
		ast_add_child(node, nil_node);
		$$ = node;
		}
	|	expression list_content {
		ast_node* node = ast_new_node(cons_cell);
		ast_add_child(node, $1);
		ast_add_child(node, $2);
		$$ = node;
	}
	;

%%

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
