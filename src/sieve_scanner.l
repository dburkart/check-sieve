%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include <iostream>
#include "sieve_parser.tab.hh"
#include "sieve_driver.hh"

static yy::location loc;

std::string multiline_buffer;
%}

%option nounput batch debug noinput reentrant

comment   #.*$
id        [a-zA-Z_][a-zA-Z0-9_]*
int       [0-9]+/[KGM]?
quant     [KGM]?
blank     [ \t]
tag       :[a-zA-Z_][a-zA-Z0-9_]*

%{
#define YY_USER_ACTION  loc.columns (yyleng);
%}

%x COMMENT MULTILINE STRING

%%

%{
    loc.step();
%}

{blank}+    loc.step();
[\n]+       loc.lines(yyleng); loc.step();

{comment}   loc.step();

"/*"                       { BEGIN(COMMENT); loc.step(); }
<COMMENT>"*/"              { BEGIN(0); loc.step(); }
<COMMENT>[\n]+             { loc.lines(yyleng); loc.step(); }
<COMMENT>[^\n]*            { loc.step(); }

["]                        { BEGIN(STRING); loc.step(); multiline_buffer = ""; }
<STRING>\\\"               { loc.step(); multiline_buffer += '"'; }
<STRING>\\                 { loc.step(); multiline_buffer += yytext; }
<STRING>["]                { loc.step(); BEGIN(0); return yy::sieve_parser::make_STRING_LITERAL( sieve::ASTString(loc, multiline_buffer), loc); }
<STRING>[^"\\\n]*          { loc.step(); multiline_buffer += yytext; }
<STRING>[\n]+              { loc.lines(yyleng); loc.step(); multiline_buffer += yytext; }

"text:"[ \t]*[\n]          { BEGIN(MULTILINE); loc.step(); loc.lines(yyleng); multiline_buffer = ""; }
"text:"[ \t]*#.*[\n]       { BEGIN(MULTILINE); loc.step(); loc.lines(yyleng); multiline_buffer = ""; }
<MULTILINE>[^\.][^\n]*[\n] { loc.lines(yyleng); loc.step(); multiline_buffer += yytext; }
<MULTILINE>[\.][\n]        { loc.lines(yyleng); loc.step(); BEGIN(0); return yy::sieve_parser::make_STRING_LITERAL( sieve::ASTString(loc, multiline_buffer), loc); }

";"             return yy::sieve_parser::make_SEMICOLON( loc );
"["             return yy::sieve_parser::make_LBRACKET( loc );
"]"             return yy::sieve_parser::make_RBRACKET( loc );
"("             return yy::sieve_parser::make_LPAREN( loc );
")"             return yy::sieve_parser::make_RPAREN( loc );
"{"             return yy::sieve_parser::make_LCURLY( loc );
"}"             return yy::sieve_parser::make_RCURLY( loc );
","             return yy::sieve_parser::make_COMMA( loc );
"true"          return yy::sieve_parser::make_TRUE( sieve::ASTBoolean(loc, true), loc );
"false"         return yy::sieve_parser::make_FALSE( sieve::ASTBoolean(loc, false), loc );
"require"       return yy::sieve_parser::make_REQUIRE( loc );
"foreverypart"  return yy::sieve_parser::make_FOREVERYPART( std::string(yytext), loc );

"if"            return yy::sieve_parser::make_IF( loc );
"elsif"         return yy::sieve_parser::make_ELSIF( loc );
"else"          return yy::sieve_parser::make_ELSE( loc );

[0-9]+/[KGM]?   {
                    errno = 0;
                    long n = strtol( yytext, NULL, 10 );
                    if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
                        driver.error (loc, "integer is out of range");
                    return yy::sieve_parser::make_NUMBER( sieve::ASTNumeric(loc, n), loc );
                }
[KGM]?      return yy::sieve_parser::make_QUANTIFIER( loc );
{id}        return yy::sieve_parser::make_IDENTIFIER( yytext, loc );
{tag}       return yy::sieve_parser::make_TAG( yytext, loc );

.           driver.error( loc, "invalid character" );
<<EOF>>     return yy::sieve_parser::make_END( loc );
%%

int yywrap(yyscan_t yyscanner)
{
    return 1;
}


void sieve::driver::scan_begin() {
    struct yyguts_t * yyg;

    yylex_init(&yyscanner);
    yyg = (struct yyguts_t*)yyscanner;
    loc = yy::location();

    if (!(yyin = fopen(file.c_str(), "r"))) {
        error( "cannot open " + file + ": " + strerror(errno) );
        exit( EXIT_FAILURE );
    }
}

void sieve::driver::scan_begin(const std::string &sieve) {
    struct yyguts_t *yyg;

    yylex_init(&yyscanner);
    yyg = (struct yyguts_t*)yyscanner;
    loc = yy::location();

    yy_scan_string(sieve.c_str(), yyscanner);
}

void sieve::driver::scan_end() {
    yylex_destroy(yyscanner);
}
