%{
	#include <stdio.h>

	#include "ast.h"

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
%token DEFINE COND NIL

%%

	statement:
		datatype
	|	definition
	|	cons
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
	;

	definition:
		'(' DEFINE SYMBOL datatype ')' {
		printf("%s has been defined\n", $3);
		}
	;

	datatype:
		NUMBER {
			printf("%d\n",$1);
			ast_node* node = ast_new_node(number);
			$$ = node;
		}
	|
		STRING {
			printf("%s\n", $1);
			//$$ = $1;
			free($1);
		}
	|
		SYMBOL {
			printf("Error: %s is undefined\n", $1);
			free($1);
		}
	;


%%

/*
	// I'm trying something out, so I'm gonna keep this here for a bit

	mylisp:
	expression
	| expression expression
	;

	definition:
	'(' DEFINE SYMBOL opperandlist expression ')' {

	}

	expression:
	'(' SYMBOL opperandlist ')' {

	}
	;

	opperandlist:
	opperand
	| opperand opperand
	;

	opperand:
		SYMBOL
	;
*/


void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
