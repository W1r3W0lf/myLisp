%{
	#include <stdio.h>

	extern int yylex();
	extern int yyparse();

	void yyerror(const char *s);
%}

%token NUBMER
%token FUNCTION_NAME
%token DEFINE

%%

	mylisp:
	expression
	| expression expression
	;

	/*I don't know if this should be a special case, or if this would fall under expression*/
	definition:
	'(' DEFINE symbol opperandlist expression ')' {

	}

	expression:
	'(' FUNCTION_NAME opperandlist ')' {

	}
	;

	opperandlist:
	opperand
	| opperand opperand
	;

	opperand:
		symbol { $$ = $1; }
	;

%%


int main(int argc, char** argv){
	yyparse();
	return 0;
}

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a error : %s\n", s);
}
