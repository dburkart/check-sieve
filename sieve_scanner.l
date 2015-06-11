%{
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "sieve_driver.hh"
#include "sieve_parser.tab.hh"

#undef yywrap
#define yywrap() 1

static yy::location loc;
%}

%option noyywrap nounput batch debug noinput

comment   #.*\n
id        [a-zA-Z][a-zA-Z]*
int       [0-9]+
blank     [ \t]
string    \"(\.|[^\"])*\"
tag       :(is|matches|contains|localpart|domain|all|over|under|lower)

%{
#define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
    loc.step();
%}

{blank}+    loc.step();
[\n]+       loc.lines(yyleng); loc.step();

{comment}   ;

";"         return yy::sieve_parser::make_SEMICOLON( loc );
"["         return yy::sieve_parser::make_LBRACKET( loc );
"]"         return yy::sieve_parser::make_RBRACKET( loc );
"("         return yy::sieve_parser::make_LPAREN( loc );
")"         return yy::sieve_parser::make_RPAREN( loc );
"{"         return yy::sieve_parser::make_LCURLY( loc );
"}"         return yy::sieve_parser::make_RCURLY( loc );
","         return yy::sieve_parser::make_COMMA( loc );
"true"      return yy::sieve_parser::make_TRUE( loc );
"false"     return yy::sieve_parser::make_FALSE( loc );
"require"   return yy::sieve_parser::make_REQUIRE( loc );

"if"        return yy::sieve_parser::make_IF( loc );
"elsif"     return yy::sieve_parser::make_ELSIF( loc );
"else"      return yy::sieve_parser::make_ELSE( loc );

{string}    return yy::sieve_parser::make_STRING_LITERAL( yytext, loc );

{int}       {
                errno = 0;
                long n = strtol( yytext, NULL, 10 );
                if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
                    driver.error (loc, "integer is out of range");
                return yy::sieve_parser::make_NUMBER( n, loc );
            }
{id}        return yy::sieve_parser::make_IDENTIFIER( yytext, loc );
{tag}       return yy::sieve_parser::make_TAG( yytext, loc );

.           driver.error( loc, "invalid character" );
<<EOF>>     return yy::sieve_parser::make_END( loc );
%%

void sieve_driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if (file.empty() || file == "-")
        yyin = stdin;
    else if (!(yyin = fopen(file.c_str(), "r"))) {
        error( "cannot open " + file + ": " + strerror(errno) );
        exit( EXIT_FAILURE );
    }
}

void sieve_driver::scan_end() {
    fclose(yyin);
}