%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {sieve_parser}

%define api.token.constructor
%define api.value.type variant

%lex-param {yyscan_t scanner} {sieve_driver &driver}
%parse-param {yyscan_t scanner} {sieve_driver &driver}

%code requires {
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

%type <int> arguments
%type <std::vector<std::string>> tag_list
%type <std::vector<std::string>> strings
%type <std::vector<std::string>> string_list

%%
%start commands;

commands : command
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
            
            if ($1 == "reject" && $2 != 1) {
                driver.error(@2, "Incorrect arguments to \"reject\" command.", "Syntax:   reject <reason: string>");
                YYABORT;
            }
            
            if ($1 == "fileinto" && $2 != 1) {
                driver.error(@2, "Incorrect arguments to \"fileinto\" command.", "Syntax:   fileinto <folder: string>");
                YYABORT;
            }
            
            if ($1 == "redirect" && $2 != 1) {
                driver.error(@2, "Incorrect arguments to \"redirect\" command.", "Syntax:   fileinto <address: string>");
                YYABORT;
            }
            
            if ($1 == "keep") {
                driver.error(@2, "Too many arguments passed to \"keep\" command.", "Syntax:   keep");
                YYABORT;
            }
            
            if ($1 == "discard") {
                driver.error(@2, "Too many arguments passed to \"discard\" command.", "Syntax:   discard");
                YYABORT;
            }
            
            if (!driver.valid_command($1)) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".");
                YYABORT;
            }
        }
    | IDENTIFIER ";"
        {
            if ($1 == "reject") {
                driver.error(@2, "Incorrect arguments to \"reject\" command.", "Syntax:   reject <reason: string>");
                YYABORT;
            }
            
            if ($1 == "fileinto") {
                driver.error(@2, "Incorrect arguments to \"fileinto\" command.", "Syntax:   fileinto <folder: string>");
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
            
            if (!driver.valid_command($1)) {
                driver.error(@1, "Unrecognized command \"" + $1 + "\".");
                YYABORT;
            }
        }
    | if_flow
    | if_flow ELSE block
    ;

block : "{" commands "}"
    ;

if_flow : IF test block
    | if_flow ELSIF test block
    ;

arguments : argument { $$ = 1; }
    | argument test { $$ = 2; }
    | argument test_list { $$ = 2; }
    | arguments argument { $$ = $1 + 1; }
    ;

argument : string_list
    | numeric
    | TAG
    ;

test_list : "(" tests ")"
    ;

tests : test
    | tests "," test
    ;

test : 
      IDENTIFIER test_list
        {
            if ($1 != "allof" && $1 != "anyof") {
                driver.error(@1, "Invalid test \"" + $1 + "\": expected \"allof\" or \"anyof\"");
                YYABORT;
            }
        }
    | IDENTIFIER tag_list string_list string_list
        {
            std::ostringstream stream;
            
            if ($1 == "address" || $1 == "envelope") {
                if ($2.size() > 3) {
                    stream << "\"" << $1 << "\" test takes up to 3 tags, but " << $2.size() << " provided.";
                    driver.error(@2, stream.str());
                    YYABORT;
                }
            } else if ($1 == "header") {
                if ($2.size() > 2) {
                    stream << "\"" << $1 << "\" test takes up to 2 tags, but " << $2.size() << " provided.";
                    driver.error(@2, stream.str()); 
                    YYABORT;
                }
            } else {
                driver.error(@1, "Unrecognized test " + $1 + ": expected allof, anyof, address, envelope, header, size, not, exists, true, or false.");
                YYABORT;
            }
        }
    | IDENTIFIER TAG numeric
        {
            if ($1 != "size") {
                driver.error(@1, "Invalid test " + $1 + ": expected \"size\"");
                YYABORT;
            }
            
            if ($2 != ":over" && $2 != ":under") {
                driver.error(@2, "\"" + $2 + "\" is invalid for the size test. Expected :over, or :under." );
                YYABORT;
            }
        }
    | IDENTIFIER test
        {
            if ($1 != "not") {
                driver.error(@1, "Invalid test " + $1 + ": expected \"not\"");
                YYABORT;
            }
        }
    | IDENTIFIER string_list
        {
            if ($1 != "exists") {
                driver.error(@1, "Invalid test " + $1 + ": expected \"exists\"");
                YYABORT;
            }
        } 
    | "true"
    | "false"
    ;

string_list : STRING_LITERAL {$$ = std::vector<std::string>(1, $1); }
    | "[" strings "]" { $$ = $2; }
    ;

strings : STRING_LITERAL {$$ = std::vector<std::string>(1, $1); }
    | strings "," STRING_LITERAL { $1.push_back($3); $$ = $1; }
    ;

tag_list : TAG { $$ = std::vector<std::string>(1, $1); }
    | tag_list TAG { $1.push_back($2); $$ = $1; }
    ;

numeric : NUMBER
    | NUMBER QUANTIFIER
    ;

%%

void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.error(l, m);
}