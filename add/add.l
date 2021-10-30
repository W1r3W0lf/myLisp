%{
	#include <stdio.h>
	#include "add.tab.h"
%}
%option noyywrap
%%
[ \t\n]		;
[0-9]+		{ yylval.ival = atoi(yytext); return NUMBER; }
.	return yytext[0];
%%

