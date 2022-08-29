%{
#include <iostream>
#include <string>
#include <stdlib.h>
#include <map>
#include <list>
#include "lex-pars.h"

using namespace std;

 pgm *root;

 int line_num = 1;

int yylex();
void yyerror(char * s);

%}

%start program

%union {
  float num;
  char *id;
  exp_node *expnode;
  list<statement *> *stmts;
  statement *st;
  pgm *prog;
}

%error-verbose

%token <num> NUMBER
%token <id> ID
%token <const int> ICONST
%token <const float> FCONST
%token <const char> CCONST
%token <const> CONST
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
%token <continue> CONTINUE
%token <continue> BREAK
%token NEWLINE
%token <cmp> EQUALS 
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token SEMICOLON 
%token COMMA
%token LE
%token GRE
%token CondEQ
%token NEQ
%token GR
%token LESS
%token AND
%token OR


%type <expnode> exp  
%type <expnode> bool_exp  
%type <stmts> stmtlist
%type <st> stmt 
%type <st> assignment 
%type <st> if_stm 
%type <st> while_stm
%type <st> for_stm 
%type <prog> program

%left PLUS MINUS
%left TIMES DIVIDED

%%

program : stmtlist { $$ = new pgm($1); root = $$; }
;

stmtlist : stmtlist NEWLINE  
	   {
             $$ = $1;
           }
         | stmtlist stmt
            { 
              $$ = $1;
              $1->push_back($2);
            }
         | stmtlist error NEWLINE
	   { 
             $$ = $1;
             yyclearin; } 
         |  
           { $$ = new list<statement *>(); }  
;

stmt: assignment
       
| PRINT ID SEMICOLON {
  $$ = new print_stmt($2);
 }

| if_stm {
    $$ = $1;
}

| while_stm {
    $$ = $1;
}

| for_stm {
    $$ = $1;
}

| stmt NEWLINE

 ;
 
assignment : ID EQUALS exp SEMICOLON { 
    $$ = new assignment_stmt($1, $3);
 }

if_stm : 
    IF LPAREN bool_exp RPAREN stmtlist
    {
        $$ = new if_stmt($3, $5, nullptr);
    }

    | IF LPAREN bool_exp RPAREN stmtlist ELSE stmtlist
    {
         $$ = new if_stmt($3, $5, new else_stmt($7));
    }
    
    | /* empty */
    ;


while_stm : WHILE LPAREN bool_exp RPAREN stmtlist{
                $$ = new while_stmt($3, $5);
                }

for_stm : FOR LPAREN assignment SEMICOLON
                bool_exp SEMICOLON
                exp SEMICOLON RPAREN stmtlist{
                    $$ = new for_stmt($3, $5, $7, $10);
                }

exp:	exp PLUS exp {
	  $$ = new plus_node($1, $3); }

	|	exp TIMES exp {
	  $$ = new times_node($1, $3); }
	
	|	NUMBER {
	  $$ = new number_node($1); }

	|       ID {
  	  $$ = new id_node($1); }

;

bool_exp : exp { $$ = $1; }
    | exp LE exp { $$ = new bool_op_node(1 , $1, $3); }
    | exp GRE exp { $$ = new bool_op_node(2 , $1, $3); }
    | exp CondEQ exp { $$ = new bool_op_node(3 , $1, $3); }
    | exp NEQ exp { $$ = new bool_op_node(4 , $1, $3); }
    | exp GR exp { $$ = new bool_op_node(5 , $1, $3); }
    | exp LESS exp { $$ = new bool_op_node(6 , $1, $3); }
    | bool_exp AND bool_exp { $$ = new bool_op_node(7 , $1, $3); }
    | bool_exp OR bool_exp { $$ = new bool_op_node(8 , $1, $3); }
    ;


%%
main()
{
  yyparse();
  //root->evaluate();
  root->print();
}

void yyerror(char * s)
{
  fprintf(stderr, "line %d: %s\n", line_num, s);
}

