%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "parse_tree.h"
#if YYDEBUG
extern int yydebug;
#endif
extern int yylex(void);
static void yyerror(const char *msg);
%}
%union {
  int num;
  char *str;
  struct pt_node *node;
}
%token <num> LITERAL_NUMBER
%token <str> LITERAL_STRING IDENTIFIER
%token <str> CLASS CONSTRUCTOR FUNCTION METHOD FIELD STATIC VAR INT CHAR BOOLEAN VOID
             TRUE FALSE NULLPO THIS LET DO IF ELSE WHILE RETURN
%token <str> '+' '-' '*' '/' '&' '|' '<' '>' '=' '~'
%type <node> program class opt_class_body class_body class_var subroutine opt_params params
             subrt_body subrt_var opt_stmts stmts stmt let_stmt if_stmt while_stmt
             do_stmt return_stmt subrt_call opt_args args expr primary
%type <str>  class_var_scope type subrt_type subrt_ret_type
%left '+' '-'
%left '*' '/'
%right '~'
%expect 18
%%
program: class { $$ = build_pt_root($1); print_pt_as_xml(stdout, $$, 0); }
       ;

class: CLASS IDENTIFIER '{' opt_class_body '}' { $$ = build_pt_class($2, $4); }
     ;

opt_class_body: /* empty */ { $$ = NULL; }
              | class_body  { $$ = $1; }
              ;

class_body: class_var ';'            { $$ = build_pt_class_body(); add_pt_class_var_to_class_body($$, $1); }
          | subroutine               { $$ = build_pt_class_body(); add_pt_subroutine_to_class_body($$, $1); }
          | class_body class_var ';' { $$ = $1; add_pt_class_var_to_class_body($$, $2); }
          | class_body subroutine    { $$ = $1; add_pt_subroutine_to_class_body($$, $2); }
          ;

class_var: class_var_scope type IDENTIFIER { $$ = build_pt_class_var($1, $2, $3); }
         | class_var ',' IDENTIFIER        { $$ = $1; add_pt_class_var_to_class_var($$, $3); }
         ;

class_var_scope: STATIC { $$ = $1; }
               | FIELD  { $$ = $1; }
               ;

type: INT        { $$ = $1; }
    | CHAR       { $$ = $1; }
    | BOOLEAN    { $$ = $1; }
    | IDENTIFIER { $$ = $1; }
    ;

subroutine: subrt_type subrt_ret_type IDENTIFIER '(' opt_params ')' '{' subrt_body '}'
            { $$ = build_pt_subroutine($1, $2, $3, $5, $8); }
          ;

subrt_type: CONSTRUCTOR { $$ = $1; }
          | FUNCTION    { $$ = $1; }
          | METHOD      { $$ = $1; }
          ;

subrt_ret_type: VOID { $$ = $1; }
              | type { $$ = $1; }
              ;

opt_params: /* empty */ { $$ = build_pt_param(NULL, NULL); }
          | params      { $$ = $1; }
          ;

params: type IDENTIFIER            { $$ = build_pt_param($1, $2); }
      | params ',' type IDENTIFIER { $$ = $1; add_pt_param_to_params($$, $3, $4); }
      ;

subrt_body: subrt_var ';'            { $$ = build_pt_subroutine_body(); add_pt_subrt_var_to_subrt_body($$, $1); }
          | opt_stmts                { $$ = build_pt_subroutine_body(); add_pt_stmt_to_subrt_body($$, $1); }
          | subrt_body subrt_var ';' { $$ = $1; add_pt_subrt_var_to_subrt_body($$, $2); }
          | subrt_body opt_stmts     { $$ = $1; add_pt_stmt_to_subrt_body($$, $2); }
          ;

subrt_var: VAR type IDENTIFIER      { $$ = build_pt_subroutine_var($2, $3); }
         | subrt_var ',' IDENTIFIER { $$ = $1; add_pt_subrt_var_to_subrt_var($$, $3); }
         ;

opt_stmts: /* empty */ { $$ = NULL; }
         | stmts       { $$ = $1; }
         ;

stmts: stmt       { $$ = build_pt_stmts(); add_pt_stmt_to_stmts($$, $1); }
     | stmts stmt { $$ = $1; add_pt_stmt_to_stmts($$, $2); }
     ;

stmt: let_stmt    { $$ = $1; }
    | if_stmt     { $$ = $1; }
    | while_stmt  { $$ = $1; }
    | do_stmt     { $$ = $1; }
    | return_stmt { $$ = $1; }
    ;

let_stmt: LET IDENTIFIER '=' expr ';'              { $$ = build_pt_let_stmt($2, $4); }
        | LET IDENTIFIER '[' expr ']' '=' expr ';' { $$ = build_pt_let_arr_stmt($2, $4, $7); }
        ;

if_stmt: IF '(' expr ')' '{' opt_stmts '}'                        { $$ = build_pt_if_stmt($3, $6); }
       | IF '(' expr ')' '{' opt_stmts '}' ELSE '{' opt_stmts '}' { $$ = build_pt_if_else_stmt($3, $6, $10); }
       ;

while_stmt: WHILE '(' expr ')' '{' opt_stmts '}' { $$ = build_pt_while_stmt($3, $6); }
          ;

do_stmt: DO subrt_call ';' { $$ = build_pt_do_stmt($2); }
       ;

return_stmt: RETURN ';'      { $$ = build_pt_return_stmt(NULL); }
           | RETURN expr ';' { $$ = build_pt_return_stmt($2); }
           ;

subrt_call: IDENTIFIER '(' opt_args ')'                { $$ = build_pt_subroutine_call(NULL, $1, $3); }
          | IDENTIFIER '.' IDENTIFIER '(' opt_args ')' { $$ = build_pt_subroutine_call($1, $3, $5); }
          ;

opt_args: /* empty */ { $$ = build_pt_args(); }
        | args        { $$ = $1; }
        ;

args: expr          { $$ = build_pt_args(); add_pt_expr_to_args($$, $1); }
    | args ',' expr { $$ = $1; add_pt_expr_to_args($$, $3); }
    ;

expr: primary             { $$ = build_pt_expr($1, NULL, NULL); }
    | primary '+' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '-' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '*' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '/' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '&' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '|' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '<' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '>' primary { $$ = build_pt_expr($1, $2, $3); }
    | primary '=' primary { $$ = build_pt_expr($1, $2, $3); }
    ;

primary: LITERAL_NUMBER          { $$ = build_pt_primary_literal_num($1); }
       | LITERAL_STRING          { $$ = build_pt_primary_literal_str($1); }
       | TRUE                    { $$ = build_pt_primary_reserved($1); }
       | FALSE                   { $$ = build_pt_primary_reserved($1); }
       | NULLPO                  { $$ = build_pt_primary_reserved($1); }
       | THIS                    { $$ = build_pt_primary_reserved($1); }
       | IDENTIFIER              { $$ = build_pt_primary_identifier($1); }
       | IDENTIFIER '[' expr ']' { $$ = build_pt_primary_arr($1, $3); }
       | subrt_call              { $$ = build_pt_primary_subrt_call($1); }
       | '(' expr ')'            { $$ = build_pt_primary_parenthesis($2); }
       | '-' primary             { $$ = build_pt_primary_unary_operation($1, $2); }
       | '~' primary             { $$ = build_pt_primary_unary_operation($1, $2); }
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
