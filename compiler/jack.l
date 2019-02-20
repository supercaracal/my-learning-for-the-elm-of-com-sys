%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"
%}

%%

"+" { return ADD; }
"-" { return SUB; }
"*" { return MUL; }
"/" { return DIV; }

"(" { return BL; }
")" { return BR; }

"\n" { return LF; }

[1-9][0-9]* {
  sscanf(yytext, "%lf", &yylval.num);
  return NUMBER;
}

[0-9]\.[0-9]+ {
  sscanf(yytext, "%lf", &yylval.num);
  return NUMBER;
}

%%

int yywrap(void) {
  return 1;
}
