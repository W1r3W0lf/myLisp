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

%token <int_number> NUBMER
%token <symbol> SYMBOL
%token <string> STRING
%token DEFINE

%%

	mylisp:
	expression
	| expression expression
	;

	/*I don't know if this should be a special case, or if this would fall under expression*/
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
		SYMBOL { $$ = $1; }
	;

%%


int main(int argc, char** argv){
	yyparse();
	return 0;
}

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
