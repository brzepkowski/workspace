%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "calc.tab.h"

%}

%%
"#".*		{return comment;} /* Ignore one-line comments */
"#"(.*\\\n)*.*	{return comment;} /*Ignore "multi-line one-line comments" */

[ \t]	; // ignore all whitespace
[0-9]+		{yylval = atoi(yytext); return NUM;}
\n		{return '\n';}
"+"		{return '+';}
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
"^"		{return '^';}
"("		{return '(';}
")"		{return ')';}
"exit"		{return QUIT;}
"quit"		{return QUIT;}

%%
