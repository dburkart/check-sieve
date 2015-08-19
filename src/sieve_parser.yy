%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {sieve_parser}

%define api.token.constructor
%define api.value.type variant

%lex-param {yyscan_t scanner} {sieve_driver &driver}
%parse-param {yyscan_t scanner} {sieve_driver &driver}

%code requires {
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
class sieve_driver;
typedef void* yyscan_t;
}

%locations
%initial-action {
    @$.begin.filename = @$.end.filename = &driver.file;
}

%define parse.trace
%define parse.error verbose

%code {
#include "sieve_driver.hh"
#include "sieve_scanner.h"
}

%define api.token.prefix {TOK_}
%token
    END 0         "end of file"
    REQUIRE       "require"
    STRING        "string"
    IF            "if"
    ELSIF         "elsif"
    ELSE          "else"
    FOREVERYPART  "foreverypart"
    SEMICOLON     ";"
    LPAREN        "("
    RPAREN        ")"
    LBRACKET      "["
    RBRACKET      "]"
    LCURLY        "{"
    RCURLY        "}"
    COMMA         ","
    TRUE          "true"
    FALSE         "false"
    QUANTIFIER    "quantifier"
  ;

%token <std::string> IDENTIFIER "identifier"
%token <std::string> TAG ":tag"
%token <std::string> STRING_LITERAL "string literal"
%token <int> NUMBER "number"

%type <int> numeric
%type <std::vector<std::string>> argument
%type <std::vector<std::string>> arguments
%type <std::vector<std::string>> test
%type <std::vector<std::string>> tests
%type <std::vector<std::string>> test_list
%type <std::vector<std::string>> strings
%type <std::vector<std::string>> string_list

%%
%start commands;

commands : END
    | command
    | commands command
    ;

command :
      REQUIRE string_list ";"
        {
            driver.set_required_modules( $2 );
        }
    | IDENTIFIER arguments ";"
        {
            if (!driver.supports_module("imap4flags") && ($1 == "addflag" || $1 == "setflag" || $1 == "removeflag" || $1 == "hasflag")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require imap4flags");
                YYABORT;
            }

            if (!driver.supports_module("variables") && ($1 == "set")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require variables");
                YYABORT;
            }

            if (!driver.supports_module("fileinto") && ($1 == "fileinto")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require fileinto");
                YYABORT;
            }

            if (!driver.supports_module("vacation") && ($1 == "vacation")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require vacation");
                YYABORT;
            }

            if (!driver.supports_module("include") && ($1 == "include")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require include");
                YYABORT;
            }

            if (!driver.supports_module("reject") && ($1 == "reject")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require reject");
                YYABORT;
            }

            if (!driver.supports_module("ereject") && ($1 == "ereject")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require ereject");
                YYABORT;
            }

            if (!driver.supports_module("foreverypart") && ($1 == "break")) {
                driver.error(@1, "Unrecognized command\"" + $1 + "\".", "Hint: require foreverypart");
                YYABORT;
            }

            if (!driver.supports_module("enclose") && ($1 == "enclose")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require enclose");
                YYABORT;
            }

            if (!driver.supports_module("extracttext") && ($1 == "extracttext")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require extracttext");
                YYABORT;
            }
            if (!driver.supports_module("replace") && ($1 == "replace")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require replace");
                YYABORT;
            }

            if ($1 == "reject" && $2.size() != 1) {
                driver.error(@2, "Incorrect arguments to \"reject\" command.", "Syntax:   reject <reason: string>");
                YYABORT;
            }

            if ($1 == "ereject" && $2.size() != 1) {
                driver.error(@2, "Incorrect arguments to \"reject\" command.", "Syntax:   ereject <reason: string>");
                YYABORT;
            }

            if ($1 == "fileinto" && ($2.size() > 2 || $2.size() < 1)) {
                driver.error(@2, "Incorrect arguments to \"fileinto\" command.", "Syntax:   fileinto [\":copy\"] <folder: string>");
                YYABORT;
            }

            if ($1 == "redirect" && ($2.size() > 2 || $2.size() < 1)) {
                driver.error(@2, "Incorrect arguments to \"redirect\" command.", "Syntax:   redirect [\":copy\"] <address: string>");
                YYABORT;
            }

            if ($1 == "break") {
                if ($2.size() != 2) {
                    driver.error(@2, "Incorrect arguments to \"break\" command.", "Syntax:   break [\":name\" string]");
                    YYABORT;
                }

                if ($2[0] != ":name") {
                    driver.error(@2, "Unrecognized tag.", "Syntax:    break [\":name\" string]");
                    YYABORT;
                }
            }

            if ($1 == "keep") {
                driver.error(@2, "Too many arguments passed to \"keep\" command.", "Syntax:   keep");
                YYABORT;
            }

            if ($1 == "discard") {
                driver.error(@2, "Too many arguments passed to \"discard\" command.", "Syntax:   discard");
                YYABORT;
            }

            if ($1 == "return") {
                driver.error(@2, "Too many arguments passed to \"return\" command.", "Syntax:   return");
                YYABORT;
            }

            if (!driver.valid_command($1)) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".");
                YYABORT;
            }
        }
    | IDENTIFIER ";"
        {
            if (!driver.supports_module("include") && ($1 == "return")) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".", "Hint: require include");
                YYABORT;
            }

            if (!driver.supports_module("foreverypart") && ($1 == "break")) {
                driver.error(@1, "Unrecognized command\"" + $1 + "\".", "Hint: require foreverypart");
                YYABORT;
            }

            if ($1 == "reject") {
                driver.error(@2, "Incorrect arguments to \"reject\" command.", "Syntax:   reject <reason: string>");
                YYABORT;
            }

            if ($1 == "fileinto") {
                driver.error(@2, "Incorrect arguments to \"fileinto\" command.", "Syntax:   fileinto [\":copy\"] <folder: string>");
                YYABORT;
            }

            if ($1 == "redirect") {
                driver.error(@2, "Incorrect arguments to \"redirect\" command.", "Syntax:   fileinto <address: string>");
                YYABORT;
            }

            if ($1 == "vacation") {
                driver.error(@2, "Incorrect arguments to \"vacation\" command.");
                YYABORT;
            }

            if ($1 == "include") {
                driver.error(@2, "Incorrect arguments to \"include\" command.");
                YYABORT;
            }

            if (!driver.valid_command($1)) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".");
                YYABORT;
            }
        }
    | FOREVERYPART block
        {
            if (!driver.supports_module("foreverypart")) {
                driver.error(@1, "Unrecognized action \"foreverypart\".", "Hint: require foreverypart");
                YYABORT;
            }
        }
    | FOREVERYPART TAG STRING_LITERAL block
        {
            if (!driver.supports_module("foreverypart")) {
                driver.error(@1, "Unrecognized action \"foreverypart\".", "Hint: require foreverypart");
                YYABORT;
            }

            if ($2 != ":name") {
                driver.error(@2, "Unrecognized tag \"" + $2 + "\".", "Syntax:    foreverypart [\":name\" string] block");
                YYABORT;
            }
        }
    | if_flow
    | if_flow ELSE block
    | ";"
    ;

block : "{" commands "}"
    | "{" "}"
    ;

if_flow : IF test block
    | if_flow ELSIF test block
    ;

arguments : argument { $$ = $1; }
    | argument test { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | argument test_list { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | arguments argument { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | test { $$ = $1; }
    | test_list { $$ = $1; }
    ;

argument : string_list { $$ = $1; }
    | numeric { $$ = std::vector<std::string>( 1, std::to_string($1) ); }
    | TAG
        {
            if ( !driver.supports_module("index") && $1 == ":index" ) {
                driver.error(@1, "Unrecognized tag \"" + $1 + "\".", "Hint: require index");
                YYABORT;
            }

            if ( !driver.supports_module("copy") && $1 == ":copy" ) {
                driver.error(@1, "Unrecognized tag \"" + $1 + "\".", "Hint: require copy");
                YYABORT;
            }

            if ( !driver.supports_module("relational") && ($1 == ":count" || $1 == ":value") ) {
                driver.error(@1, "Unrecognized tag \"" + $1 + "\".", "Hint: require relational");
                YYABORT;
            }

            $$ = std::vector<std::string>(1, $1);
        }
    ;

test_list : "(" tests ")" { $$ = $2; }
    ;

tests : test { $$ = $1; }
    | tests "," test { $1.insert($1.end(), $3.begin(), $3.end()); $$ = $1; }
    ;

test :
     IDENTIFIER arguments {
        std::transform($1.begin(), $1.end(), $1.begin(), ::tolower);
        if (!driver.valid_test($1)) {
            driver.error(@1, "Unknown test \"" + $1 + "\".");
            YYABORT;
        }

        if ( !driver.supports_module("imap4flags") && $1 == "hasflag" ) {
            driver.error(@1, "Unrecognized test \"" + $1 + "\".", "Hint: require imap4flags");
            YYABORT;
        }

        if ( !driver.supports_module("variables") && $1 == "string" ) {
            driver.error(@1, "Unrecognized test \"" + $1 + "\".", "Hint: require variables");
            YYABORT;
        }

        if ( !driver.supports_module("date") && ($1 == "date" || $1 == "currentdate") ) {
            driver.error(@1, "Unrecognized test \"" + $1 + "\".", "Hint: require date");
            YYABORT;
        }

        if ( !driver.supports_module("body") && ($1 == "body") ) {
            driver.error(@1, "Unrecognized test \"" + $1 + "\".", "Hint: require body");
            YYABORT;
        }

        $2.push_back($1);
     }
    | "true" { $$ = std::vector<std::string>(1, "true"); }
    | "false" { $$ = std::vector<std::string>(1, "false"); }
    ;

string_list : STRING_LITERAL {$$ = std::vector<std::string>(1, $1); }
    | "[" strings "]" { $$ = $2; }
    ;

strings : STRING_LITERAL {$$ = std::vector<std::string>(1, $1); }
    | strings "," STRING_LITERAL { $1.push_back($3); $$ = $1; }
    ;

numeric : NUMBER { $$ = $1; }
    | NUMBER QUANTIFIER { $$ = $1; }
    ;

%%

void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.error(l, m);
}
