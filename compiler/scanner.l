%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#define YY_NO_INPUT 1
#define YY_NO_UNPUT 1
#define YYERROR_VERBOSE 1
#define ALLOC_STRING(src_str) ((char *) calloc(1, strlen(src_str) + 1))
static void yyerror(const char *msg);
%}
REGEXP_NUMBER     ([1-9][0-9]*|0)
REGEXP_STRING     \"([^"\\]|\\.)*\"
REGEXP_IDENTIFIER [a-zA-Z_][a-zA-Z0-9_]*
%option yylineno
%option noyywrap
%x ML_COMMENT
%x OL_COMMENT
%%
"/*"                       BEGIN(ML_COMMENT);
<ML_COMMENT>[^*\n]*
<ML_COMMENT>[^*\n]*\n
<ML_COMMENT>"*"+[^*/\n]*
<ML_COMMENT>"*"+[^*/\n]*\n
<ML_COMMENT><<EOF>>        yyerror("Reached to EOF in multi-line comment.");
<ML_COMMENT>"*"+"/"        BEGIN(INITIAL);

"//"                       BEGIN(OL_COMMENT);
<OL_COMMENT>[^\n]*\n       BEGIN(INITIAL);

class       return CLASS;
constructor return CONSTRUCTOR;
function    return FUNCTION;
method      return METHOD;
field       return FIELD;
static      return STATIC;
var         return VAR;
int         return INT;
char        return CHAR;
boolean     return BOOLEAN;
void        return VOID;
true        return TRUE;
false       return FALSE;
null        return NULLPO;
this        return THIS;
let         return LET;
do          return DO;
if          return IF;
else        return ELSE;
while       return WHILE;
return      return RETURN;

"{" return '{';
"}" return '}';
"(" return '(';
")" return ')';
"[" return '[';
"]" return ']';
"." return '.';
"," return ',';
";" return ';';
"+" return '+';
"-" return '-';
"*" return '*';
"/" return '/';
"&" return '&';
"|" return '|';
"<" return '<';
">" return '>';
"=" return '=';
"~" return '~';

{REGEXP_NUMBER} {
  sscanf(yytext, "%d", &yylval.num);
  return LITERAL_NUMBER;
}

{REGEXP_STRING} {
  yylval.str = ALLOC_STRING(yytext);
  sscanf(yytext, "%s", yylval.str);
  return LITERAL_STRING;
}

{REGEXP_IDENTIFIER} {
  yylval.str = ALLOC_STRING(yytext);
  sscanf(yytext, "%s", yylval.str);
  return IDENTIFIER;
}

[ \t\r\n] ;

. {
  yyerror("Unknown token was found.");
}
%%
static void yyerror(const char *msg) {
  fprintf(stderr, "[ERROR] %s (%d: %s)\n", msg, yylineno, yytext);
}
