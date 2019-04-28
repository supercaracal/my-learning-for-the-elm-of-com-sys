%{
#include <stdio.h>
#include <stdlib.h>
#if YYDEBUG
extern int yydebug;
#endif
extern int yylex(void);
static void yyerror(const char *msg);
%}
%union {
  int num;
  char *str;
}
%token <num> LITERAL_NUMBER
%token <str> LITERAL_STRING IDENTIFIER
%token <str> CLASS CONSTRUCTOR FUNCTION METHOD FIELD STATIC VAR INT CHAR BOOLEAN VOID
             TRUE FALSE NULLPO THIS LET DO IF ELSE WHILE RETURN
%left '+' '-'
%left '*' '/'
%right '~'
%%
program: class
       ;

class: CLASS IDENTIFIER '{' opt_class_body '}'
     ;

opt_class_body: /* empty */
              | class_body
              ;

class_body: class_var ';'
          | subroutine
          | class_body class_var ';'
          | class_body subroutine
          ;

class_var: class_var_scope type IDENTIFIER
         | class_var ',' IDENTIFIER
         ;

class_var_scope: STATIC
               | FIELD
               ;

type: INT
    | CHAR
    | BOOLEAN
    | IDENTIFIER
    ;

subroutine: subrt_type subrt_ret_type IDENTIFIER '(' opt_params ')' '{' subrt_body '}'
          ;

subrt_type: CONSTRUCTOR
          | FUNCTION
          | METHOD
          ;

subrt_ret_type: VOID
              | type
              ;

opt_params: /* empty */
          | params
          ;

params: type IDENTIFIER
      | params ',' type IDENTIFIER
      ;

subrt_body: subrt_var ';'
          | opt_stmts
          | subrt_body subrt_var ';'
          | subrt_body opt_stmts
          ;

subrt_var: VAR type IDENTIFIER
         | subrt_var ',' IDENTIFIER
         ;

opt_stmts: /* empty */
         | stmts
         ;

stmts: stmt
     | stmts stmt
     ;

stmt: let_stmt
    | if_stmt
    | while_stmt
    | do_stmt
    | return_stmt
    ;

let_stmt: LET IDENTIFIER '=' expr ';'
        | LET IDENTIFIER '[' expr ']' '=' expr ';'
        ;

if_stmt: IF '(' expr ')' '{' opt_stmts '}'
       | IF '(' expr ')' '{' opt_stmts '}' ELSE '{' opt_stmts '}'
       ;

while_stmt: WHILE '(' expr ')' '{' opt_stmts '}'
          ;

do_stmt: DO subrt_call ';'
       ;

return_stmt: RETURN ';'
           | RETURN expr ';'
           ;

subrt_call: IDENTIFIER '(' opt_args ')'
          | IDENTIFIER '.' IDENTIFIER '(' opt_args ')'
          ;

opt_args: /* empty */
        | args
        ;

args: expr
    | args ',' expr
    ;

expr: primary
    | primary '+' primary
    | primary '-' primary
    | primary '*' primary
    | primary '/' primary
    | primary '&' primary
    | primary '|' primary
    | primary '<' primary
    | primary '>' primary
    | primary '=' primary
    ;

primary: LITERAL_NUMBER
       | LITERAL_STRING
       | TRUE
       | FALSE
       | NULLPO
       | THIS
       | IDENTIFIER
       | IDENTIFIER '[' expr ']'
       | subrt_call
       | '(' expr ')'
       | '-' primary
       | '~' primary
       ;
%%
int main(int argc, char *arg[]) {
  int ret;

#if YYDEBUG
  yydebug = 1;
#endif

#ifdef CHAPTER10_XML_TOKEN
  fprintf(stdout, "<tokens>\n");
#endif
  ret = yyparse();
#ifdef CHAPTER10_XML_TOKEN
  fprintf(stdout, "</tokens>");
#endif

  exit(ret);
}

static void yyerror(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}
