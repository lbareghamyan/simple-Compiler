%{
	#include "symtab.c"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	extern FILE *yyin;
	extern FILE *yyout;
	extern int lineno;
	extern int yylex();
	void yyerror();
%}

%start program

%token <num> NUMBER
%token <id> ID
%token ICONST FCONST CCONST
%token <char> CHAR
%token <bool> BOOL
%token <int> INT
%token <double> DOUBLE
%token <true> TRUE
%token <false> FALSE
%token <if> IF
%token <else> ELSE
%token <print> PRINT
%token <while> WHILE
%token <for> FOR
%token <return> RETURN
%token CONTINUE
%token BREAK
%token <const> CONST
%token NEWLINE
%token <cmp> EQUALS 
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token SEMICOLON
%token COMMA
%token LESS GR LE GRE CondEQ NEQ
%token AND OR


%left PLUS MINUS
%left TIMES DIVIDED

%%

program: declarations stmts ;

declarations: declarations declaration | declaration;

declaration: type varNames SEMICOLON ;

type: INT | CHAR | DOUBLE

varNames: var | varNames COMMA var ;

var: ID | ID array ;

array: array LBRACK ICONST RBRACK | LBRACK ICONST RBRACK ;

stmts: stmts stmt | stmt; 

stmt : if_stmt | for_stmt | while_stmt| assigment |
	CONTINUE SEMICOLON | BREAK SEMICOLON | RETURN SEMICOLON
;

if_stmt:
		IF LPAREN expression RPAREN tail optional_else
;

optional_else: ELSE tail | /* empty */ ;



 

for_stmt: FOR LPAREN expression SEMICOLON expression SEMICOLON expression RPAREN tail ;
 

while_stmt: WHILE LPAREN expression RPAREN tail ;

tail: stmt SEMICOLON | LBRACE stmt RBRACE ;

expression: 
    expression PLUS expression |
    expression MINUS expression |
    expression TIMES expression |
    expression DIVIDED expression |
    expression OR expression |
    expression AND expression |
    expression CondEQ expression |
    expression LE expression |
    expression GRE expression |
    expression NEQ expression |
    expression GR expression |
    expression LESS expression |
    LPAREN expression RPAREN |
    var
;

;

assigment: var EQUALS expression SEMICOLON ; 




%%

void yyerror ()
{
  fprintf(stderr, "Syntax error at line %d\n", lineno);
  exit(1);
}
 
int main (int argc, char *argv[]){
 
	// initialize symbol table
	init_hash_table();
 
	// parsing
	int flag;
	yyin = fopen(argv[1], "r");
	flag = yyparse();
	fclose(yyin);
 
	// symbol table dump
	yyout = fopen("symtab_dump.out", "w");
	symtab_dump(yyout);
	fclose(yyout);	
 
	return flag;
}