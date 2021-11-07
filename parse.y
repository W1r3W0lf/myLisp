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

%token <ast> NUMBER
%token <ast> SYMBOL
%token <string> STRING
%type <ast> datatype
%type <ast> cons
%type <ast> list
%type <ast> definition

%token DEFINE COND NIL

%%

	statement:
		definition
	|	condition
	|	datatype
	|	cons
	;

	definition:
		'(' DEFINE SYMBOL datatype ')' {
		ast_node* node = ast_new_node(definition);
		ast_add_child(node, $3);
		ast_add_child(node, $4);
		$$ = node;
		ast = node;
		printf("DEFINING %s",$3->value.symbol);
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
			//$$ = node;
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
	|	list datatype {
		ast_node* node = ast_new_node(cons_cell);
		ast_add_child(node, $1);
		ast_add_child(node, $2);
		$$ = node;
	}
	|	cons
	|	list cons
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
			printf("Symbol %s", node->value.symbol);
			$$ = node;
			free($1);
		}
	|	NIL {
			ast_node* node = ast_new_node(nil);
			$$ = node;
	}
	;

%%

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
