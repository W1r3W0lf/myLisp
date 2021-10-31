%{
	#include <stdio.h>
	#include "mylisp.tab.h"
%}

%%
-?[0-9]+				{ yyval.int_number = atoi(yytext); return NUMBER }
[a-zA-Z]+[a-zA-Z0-9-_]* { yyval.symbol = strdup(yytext); return SYMBOL; }
dfine					{ return DEFINE; }
[ \t\n]					;
['()"]					{ return yytext[0]; }
%%


