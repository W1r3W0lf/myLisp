%{
	#include <stdio.h>
	#include <string.h>

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
			$$ = node;
			free($1);
		}
	|	NIL {
			ast_node* node = ast_new_node(nil);
			$$ = node;
	}
	;

%%
/*
	Defining will be done by the intrpriter
	
	definition:
		'(' DEFINE SYMBOL datatype ')' {
		printf("%s has been defined\n", $3);
		}
	;
*/

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
