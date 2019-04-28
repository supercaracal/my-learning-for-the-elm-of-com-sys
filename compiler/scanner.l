%{
#include <stdio.h>
#include <string.h>
#include "y.tab.h"
#define YY_NO_INPUT 1
#define YY_NO_UNPUT 1
#define YYERROR_VERBOSE 1
#define ALLOC_STRING(src_str) ((char *) calloc(1, strlen(src_str) + 1))
#ifdef CHAPTER10_XML_TOKEN
#define P_XML_KW(str) (fprintf(stdout, "<keyword> %s </keyword>\n", str))
#define P_XML_SYM(str) (fprintf(stdout, "<symbol> %s </symbol>\n", str))
#else
#define P_XML_KW(str) ;
#define P_XML_SYM(str) ;
#endif
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

class       P_XML_KW(yytext); return CLASS;
constructor P_XML_KW(yytext); return CONSTRUCTOR;
function    P_XML_KW(yytext); return FUNCTION;
method      P_XML_KW(yytext); return METHOD;
field       P_XML_KW(yytext); return FIELD;
static      P_XML_KW(yytext); return STATIC;
var         P_XML_KW(yytext); return VAR;
int         P_XML_KW(yytext); return INT;
char        P_XML_KW(yytext); return CHAR;
boolean     P_XML_KW(yytext); return BOOLEAN;
void        P_XML_KW(yytext); return VOID;
true        P_XML_KW(yytext); return TRUE;
false       P_XML_KW(yytext); return FALSE;
null        P_XML_KW(yytext); return NULLPO;
this        P_XML_KW(yytext); return THIS;
let         P_XML_KW(yytext); return LET;
do          P_XML_KW(yytext); return DO;
if          P_XML_KW(yytext); return IF;
else        P_XML_KW(yytext); return ELSE;
while       P_XML_KW(yytext); return WHILE;
return      P_XML_KW(yytext); return RETURN;

"{" P_XML_SYM(yytext); return '{';
"}" P_XML_SYM(yytext); return '}';
"(" P_XML_SYM(yytext); return '(';
")" P_XML_SYM(yytext); return ')';
"[" P_XML_SYM(yytext); return '[';
"]" P_XML_SYM(yytext); return ']';
"." P_XML_SYM(yytext); return '.';
"," P_XML_SYM(yytext); return ',';
";" P_XML_SYM(yytext); return ';';
"+" P_XML_SYM(yytext); return '+';
"-" P_XML_SYM(yytext); return '-';
"*" P_XML_SYM(yytext); return '*';
"/" P_XML_SYM(yytext); return '/';
"&" P_XML_SYM("&amp;"); return '&';
"|" P_XML_SYM(yytext); return '|';
"<" P_XML_SYM("&lt;"); return '<';
">" P_XML_SYM("&gt;"); return '>';
"=" P_XML_SYM(yytext); return '=';
"~" P_XML_SYM(yytext); return '~';

{REGEXP_NUMBER} {
  sscanf(yytext, "%d", &yylval.num);
#ifdef CHAPTER10_XML_TOKEN
  fprintf(stdout, "<integerConstant> %s </integerConstant>\n", yytext);
#endif
  return LITERAL_NUMBER;
}

{REGEXP_STRING} {
  char *text = ++yytext;
  text[strlen(text) - 1] = '\0';
  yylval.str = ALLOC_STRING(text);
  sscanf(text, "%s", yylval.str);
#ifdef CHAPTER10_XML_TOKEN
  fprintf(stdout, "<stringConstant> %s </stringConstant>\n", text);
#endif
  return LITERAL_STRING;
}

{REGEXP_IDENTIFIER} {
  yylval.str = ALLOC_STRING(yytext);
  sscanf(yytext, "%s", yylval.str);
#ifdef CHAPTER10_XML_TOKEN
  fprintf(stdout, "<identifier> %s </identifier>\n", yytext);
#endif
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
