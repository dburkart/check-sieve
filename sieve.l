%{
/*
 * scan.l
 */

#include <stdio.h>
#include "sieve.tab.h"
int line_no = 1;

%}

%%

"require"           { yylval = yytext; return REQUIRE; }

"if"                { yylval = yytext; return IF; }
"elsif"             { yylval = yytext; return ELSIF; }
"else"              { yylval = yytext; return ELSE; }

"keep"              { yylval = yytext; return IDENTIFIER_NO_STRINGS; }
"fileinto"          { yylval = yytext; return IDENTIFIER_SINGLE_STRING; }
"redirect"          { yylval = yytext; return IDENTIFIER_SINGLE_STRING; }
"discard"           { yylval = yytext; return IDENTIFIER_NO_STRINGS; }
"stop"              { yylval = yytext; return IDENTIFIER_NO_STRINGS; }

"setflag"           { yylval = yytext; return FLAG_COMMAND; }
"addflag"           { yylval = yytext; return FLAG_COMMAND; }
"removeflag"        { yylval = yytext; return FLAG_COMMAND; }
"hasflag"           { yylval = yytext; return HASFLAG_COMMAND; }

"address"           { yylval = yytext; return ADDRESS; }
"allof"             { yylval = yytext; return ALLOF; }
"anyof"             { yylval = yytext; return ANYOF; }
"envelope"          { yylval = yytext; return ENVELOPE; }
"exists"            { yylval = yytext; return EXISTS; }
"true"              { yylval = yytext; return TRUE; }
"false"             { yylval = yytext; return FALSE; }
"header"            { yylval = yytext; return HEADER; }
"not"               { yylval = yytext; return NOT; }
"size"              { yylval = yytext; return SIZE; }

:(over|under)       { yylval = yytext; return SIZE_TAG; }
:(is|matches|contains|localpart|domain|all) { yylval = yytext; return TAG; }

#.*\n           ;
[0-9]+              { yylval = atoi(yytext); return NUMBER; }
[0-9]+/[KGM]?       { yylval = atoi(yytext); return NUMBER; }
[KMG]?              { yylval = yytext; return QUANTIFIER; }

\"(.|[^\"])*\"   { return STRING_LITERAL; }


[ \t]           ;

\n              line_no++;

.               return yytext[0];

%%
