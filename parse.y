%{
	#include <stdio.h>

	extern int yylex();
	extern int yyparse();

	void yyerror(const char *s);
%}

%union{
	char* symbol;
	int int_number;
	char* string;
}

%token <int_number> NUMBER
%token <symbol> SYMBOL
%token <string> STRING
%token DEFINE

%%


	mylisp:
	NUMBER {
		printf("y %d\n", $1);
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
