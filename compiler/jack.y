%{
#include <stdio.h>
#include <stdlib.h>
#define YYDEBUG 1

extern char *yytext;
extern FILE *yyin;
extern int yyparse(void);
int yyerror(char const *str);
%}

%union {
  double num;
}
%token <num> NUMBER
%token ADD SUB MUL DIV BL BR LF
%type <num> expr primary
%left ADD SUB
%left MUL DIV

%%

lines: line
     | lines line
     ;

line: expr LF { printf("= %lf\n", $1); }
    ;

expr: expr ADD expr { $$ = $1 + $3; }
    | expr SUB expr { $$ = $1 = $3; }
    | expr MUL expr { $$ = $1 * $3; }
    | expr DIV expr { $$ = $1 / $3; }
    | primary
    ;

primary: NUMBER
       | BL expr BR { $$ = $2; }

%%

int yyerror(char const *str) {
  fprintf(stderr, "[Error] Failed on yyerror(): %s\n", yytext);
  return 0;
}

int main(int argc, char *arg[]) {
  yyin = stdin;
  if (yyparse()) {
    fprintf(stderr, "[Error] Failed on yyparse()\n");
    exit(1);
  }
  exit(0);
}
