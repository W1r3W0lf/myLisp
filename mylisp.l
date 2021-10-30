%{
	#include <stdio.h>
	#include "mylisp.tab.h"

%}

%%
[ \t\n]		;
-?[0-9]+	{ yyval = atoi(yytext); return NUMBER }
.			{ return yytext[0]; }
%%


