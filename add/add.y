%{
	#include <stdio.h>

	extern int yylex();
	extern int yyparse();

	void yyerror(const char *s);
%}



%token NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type expression

%%

	problemset:
	problem problem
	| problem
	;

	problem:
		expression '=' {
		printf("= %d\n", $1);
		}
	;

	expression:
		expression '+' expression { $$ = $1 + $3; }
	|	expression '-' expression { $$ = $1 - $3; }
	|	expression '*' expression { $$ = $1 * $3; }
	|	expression '/' expression {
			if ($3 == 0)
				yyerror("devide by zero");
			else
				$$ = $1 / $3;
		}
	|	'-' expression %prec UMINUS { $$ = -$2; }
	|	'(' expression ')' { $$ = $2; }
	|	NUMBER { $$ = $1; }
	;

%%


int main(int argc, char** argv){

	yyparse();
	return 0;
}

void yyerror(const char *s){
	fprintf(stderr, "Ooops there was a compiler error : %s\n", s);
}
