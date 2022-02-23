%{
	#include "parse.h"
	#include <stdio.h>
%}

%option noinput nounput

%%
define						{ return DEFINE; }
if							{ return IF; }
lambda						{ return LAMBDA; }
'							{ return QUOTE; }
-?[0-9]+					{ yylval.int_number = atoi(yytext); return NUMBER; }
[a-zA-Z+\-*/]+[a-zA-Z0-9]*	{ yylval.symbol = strdup(yytext); return SYMBOL; }
['"][a-zA-Z0-9]*['"]		{ yylval.string = strdup(yytext); return STRING; }
[ \t\n]						;
[()"\.]					{ return yytext[0]; }
%%


