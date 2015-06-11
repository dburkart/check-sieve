%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {sieve_parser}

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
#include <string>
#include <sstream>
class sieve_driver;
}

%param { sieve_driver &driver }

%locations
%initial-action {
    @$.begin.filename = @$.end.filename = &driver.file;
}

%define parse.trace
%define parse.error verbose

%code {
#include "sieve_driver.hh"
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
  ;

%token <std::string> IDENTIFIER "identifier"
%token <std::string> TAG ":tag"
%token <std::string> STRING_LITERAL "string literal"
%token <int> NUMBER "number"

%type <int> tag_list
%type <int> arguments

%printer { yyoutput << $$; } <*>;

%%
%start commands;

commands : command
    | commands command
    ;

command : 
      IDENTIFIER arguments block
    | REQUIRE string_list ";"
    | IDENTIFIER arguments ";"
        {
            if ($1 != "keep" && $1 != "discard" && $1 != "redirect" && $1 != "reject" && $1 != "fileinto") {
                driver.error(@1, "Unknown command \"" + $1 + "\"");
                exit( EXIT_FAILURE );
            }
        }
    | IDENTIFIER ";"
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
    | NUMBER
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
                driver.error("Invalid test \"" + $1 + "\": expected \"allof\" or \"anyof\"");
                exit( EXIT_FAILURE );
            }
        }
    | IDENTIFIER tag_list string_list string_list
        {
            std::ostringstream stream;
            
            if ($1 == "address" || $1 == "envelope") {
                if ($2 > 3) {
                    stream << "\"" << $1 << "\" test takes up to 3 tags, but " << $2 << " provided.";
                    driver.error(stream.str());
                    exit( EXIT_FAILURE );
                }
            } else if ($1 == "header") {
                if ($2 > 2) {
                    stream << "\"" << $1 << "\" test takes up to 2 tags, but " << $2 << " provided.";
                    driver.error(stream.str()); 
                    exit( EXIT_FAILURE );
                }
            } else {
                driver.error("Unrecognized test " + $1 + ": expected allof, anyof, address, envelope, header, size, not, exists, true, or false.");
                exit( EXIT_FAILURE );
            }
        }
    | IDENTIFIER TAG NUMBER
        {
            if ($1 != "size") {
                driver.error("Invalid test " + $1 + ": expected \"size\"");
                exit( EXIT_FAILURE );
            }
            
            if ($2 != ":over" && $2 != ":under") {
                driver.error("\"" + $2 + "\" is invalid for the size test. Expected :over, or :under." );
                exit( EXIT_FAILURE );
            }
        }
    | IDENTIFIER test
        {
            if ($1 != "not") {
                driver.error("Invalid test " + $1 + ": expected \"not\"");
                exit( EXIT_FAILURE );
            }
        }
    | IDENTIFIER string_list
        {
            if ($1 != "exists") {
                driver.error("Invalid test " + $1 + ": expected \"exists\"");
                exit( EXIT_FAILURE );
            }
        } 
    | "true"
    | "false"
    ;

string_list : STRING_LITERAL
    | "[" strings "]"
    ;

strings : STRING_LITERAL
    | strings "," STRING_LITERAL
    ;

tag_list : TAG { $$ = 1; }
    | tag_list TAG { $$ = $1 + 1; }
    ;

%%

void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.error(l, m);
}