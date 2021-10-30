%{
	#include <stdio.h>
	#include "add.tab.h"
%}
%%
[\t\n ]		;
[0-9]+		{ yylval = atoi(yytext); return NUMBER; }
.			{ return yytext[0]; }
%%

