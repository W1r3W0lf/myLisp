%{
	#include <stdio.h>

	extern int yylex();
	//extern int yyparese();

	void yyerror(const char *s);
%}


%union {
	int ival;
}

%token <ival> NUMBER
%left '-' '+'

%type <ival> expression

%%

	expression :	expression '+' expression {
					printf("%s + %s", $1, $3);
					$$ = $1 + $3; }
		|			expression '-' expression {
					printf("%s - %s", $1, $3);
					$$ = $1 - $3; }
		|			NUMBER { 
					printf("%s\n", $1);
					$$ = $1; }
	;

%%


int main(int argc, char** argv){
	
	yylex();
	return 0;
}

void yyerror(const char *s){
	fprintf(stderr, "Ooops a parsor error: %s\n", s);
}
