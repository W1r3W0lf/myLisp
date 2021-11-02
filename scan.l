%{
	#include <stdio.h>
	#include "parse.h"
%}

%option noinput nounput

%%
define					{ return DEFINE; }
-?[0-9]+				{ yylval.int_number = atoi(yytext); return NUMBER; }
[a-zA-Z]+[a-zA-Z0-9]*	{ yylval.symbol = strdup(yytext); return SYMBOL; }
['"][a-zA-Z0-9]*['"]	{ yylval.string = strdup(yytext); return STRING; }
[ \t\n]					;
['()"\.]					{ return yytext[0]; }
%%


