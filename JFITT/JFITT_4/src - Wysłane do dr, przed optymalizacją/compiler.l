%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "main.h"
#include "compiler.tab.h"

#define YY_DECL int yylex()

#include "compiler.tab.h"
#include "main.h"

/* handle locations */
int yycolumn = 1;

#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
						yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
						yycolumn += yyleng;
    


%}

%x COMMENT

%%

[ \t]	; // ignore all whitespace

"["	BEGIN(COMMENT);
<COMMENT>[^]\n]*	//Wszystko, co nie jest "]" lub \n
<COMMENT>"]"		{BEGIN(INITIAL);} 

\n		{yylineno++;}	/*****Przejście do nowej linii**/


"DECLARE"	{return DECLARE;}	/*****Słowa TOKENY**************/
"IN"		{return IN;}
"END"		{return END;}

"IF"		{return IF;}
"THEN"		{return THEN;} 
"ELSE"		{return ELSE;}
"ENDIF"		{return ENDIF;}

"WHILE"		{return WHILE;}
"DO"		{return DO;}
"ENDWHILE"	{return ENDWHILE;}

"FOR"		{return FOR;}
"DOWN"		{return DOWN;}
"FROM"		{return FROM;}
"TO"		{return TO;}
"ENDFOR"	{return ENDFOR;}

"GET"		{return GET;}
"PUT"		{return PUT;}


"+"		{return '+';}	/*****Operatory matematyczne****/
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
"%"		{return '%';}
"("		{return '(';}
")"		{return ')';}


"="		{return '=';}	/*****Operatory warunkowe*******/
"!="	{return DIFFERENT;}
"<"		{return '<';}
">"		{return '>';}
"<="	{return SMALLEREQUAL;}
">="	{return BIGGEREQUAL;}


":="	{return ASSIGN;}	/*****':=' ';'******************/
";"		{return ';';}


[0-9]+		{ 	struct node *children[0];
				yylval.node_pointer = add_node(0, yytext, children);
				yylval.node_pointer->type = strdup("NUMBER");
			  	return num;}


[_a-z]+		{	struct node *children[0];
				yylval.node_pointer = add_node(0, yytext, children);
				yylval.node_pointer->type = strdup("ID");
				return pidentifier;}

%%