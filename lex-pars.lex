%{
#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "symtab.h"
	#include "parser.tab.h"
	extern FILE *yyin;
	extern FILE *yyout;
 
	extern int line_num = 1; // initialize to 1
	void ret_print(char *token_type);
	void yyerror();

%}

DIGIT [0-9]
LETTER [a-zA-Z]
print	[ -~]

ID {LETTER}[0-9a-zA-Z]*
ICONST	"0"|[1-9]{DIGIT}*
FCONST	"0"|{DIGIT}*"."{DIGIT}+
CCONST	(\'{print}\')|(\'\\[nftrbv]\')

%%

"char"  { printf("<'char' , keyword>"); return CHAR; }

"bool"  { printf("<'bool' , keyword>"); return BOOL; }

"int"  { printf("<'int' , keyword>"); return INT; }

"double"  { printf("<'double' , keyword>"); return DOUBLE; }

"return"	{printf("<'return ' , RETURN>\n"); return RETURN;}

"continue"	{printf("<'continue' , CONTINUE>\n"); return CONTINUE;}

"break"		{printf("<'break' , BREAK>\n"); return BREAK;}

"false"   { printf("<'false' , keyword>"); return FALSE; }

"true"   { printf("<'true' , keyword>"); return TRUE; }

"while"   { printf("<'while' , keyword>"); return WHILE; }

"for"   { printf("<'for' , keyword>"); return FOR; }

"if"   { printf("<'if' , keyword>"); return IF; }

"else"   { printf("<'else' , keyword>"); return ELSE; }

"print"   { printf("<'print' , keyword>"); return PRINT; }

"const" {printf("<'const' , CONST>\n"); return CONST;}

{ID} 			{
					// insert identifier into symbol table
					insert(yytext, strlen(yytext), UNDEF, lineno);
					return ID;
				}
{ICONST} 		{ return ICONST; }
{FCONST} 		{ return FCONST; }
{CCONST} 		{ return CCONST; }


{DIGIT}+  { printf("<num, NUMBER>\n"); yylval.num = atof(yytext); return NUMBER; }


[ \t\f\r]		 // ignore white space  


"\n"    { printf("<'\\n' , NEWLINE>\n"); line_num++; return NEWLINE; }

"*"		{ printf("<'*' , TIMES>\n"); return TIMES;  }

"/"		{ printf("<'/' , DIVIDED>\n"); return DIVIDED;  }

"+"		{ printf("<'+' , PLUS>\n"); return PLUS;  }

"-"		{ printf("<'-' , MINUS>\n"); return MINUS;  }

"="     { printf("<'=' , EQUALS>\n"); return EQUALS; }

";"     { printf("<';' , SEMICOLON>\n"); return SEMICOLON; }

","     { printf("<',' , COMMA>\n"); return COMMA; }

"("     { printf("<'(' , LPAREN>\n"); return LPAREN; }

")"     { printf("<')' , RPAREN>\n"); return RPAREN; }

"]"		{ printf("[')' , RPAREN>\n"); return LBRACK; }

"["		{ printf("]')' , RPAREN>\n"); return RBRACK; }

"{"		{ printf("{')' , RPAREN>\n"); return LBRACE; }

"}"		{ printf("}')' , RPAREN>\n"); return RBRACE; }

"<="	{ printf("<'<=' , LE>\n"); return LE; }

">="	{ printf("<'>=' , GRE>\n"); return GRE; }

"=="	{ printf("<'==' , CondEQ>\n"); return CondEQ; }

"!="	{ printf("<'!=' , NEQ>\n"); return NEQ; }

">"		{ printf("<'>' , GR>\n"); return GR; }

"<"		{ printf("<'<' , LESS>\n"); return LESS; }

"&&"	{ printf("<'&&' , AND>\n"); return AND; }

"||"	{ printf("<'||' , OR>\n"); return OR; }

. { printf("error token: %s on line %d\n", yytext, line_num); yymore(); }

%%


