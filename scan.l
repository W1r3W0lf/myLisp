%{
	#include <stdio.h>
	#include "parse.h"
%}

%%
-?[0-9]+				{ yylval.int_number = atoi(yytext); return NUMBER; }
[a-zA-Z]+[a-zA-Z0-9]* { yylval.symbol = strdup(yytext); return SYMBOL; }
[ \t\n]					;
['()"]					{ return yytext[0]; }
%%


