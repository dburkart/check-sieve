%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name {sieve_parser}

%define api.token.constructor
%define api.value.type variant

%lex-param {yyscan_t scanner} {sieve::driver &driver}
%parse-param {yyscan_t scanner} {sieve::driver &driver}

%code requires {
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>

#include "AST.hh"

namespace sieve {
    class driver;
}

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
#include "sieve_scanner.hh"
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
    QUANTIFIER    "quantifier"
  ;


%token <std::string> IDENTIFIER "identifier"
%token <std::string> FOREVERYPART "foreverypart"
%token <std::string> TAG ":tag"
%token <sieve::ASTNumeric> NUMBER "number"
%token <sieve::ASTString> STRING_LITERAL
%token <sieve::ASTBoolean> TRUE "true"
%token <sieve::ASTBoolean> FALSE "false"

%type <sieve::ASTNumeric> numeric
%type <std::vector<sieve::ASTNode *>> argument
%type <std::vector<sieve::ASTNode *>> arguments
%type <std::vector<sieve::ASTNode *>> test
%type <std::vector<sieve::ASTNode *>> tests
%type <std::vector<sieve::ASTNode *>> test_list

%type <std::vector<sieve::ASTNode *>> strings
%type <std::vector<sieve::ASTNode *>> string_list

%type <sieve::ASTNode *> command

%%
%start commands;

commands : END
    | command
    | commands command
    ;

command :
      REQUIRE string_list ";"
        {
            sieve::ASTRequire *require = new sieve::ASTRequire(@1);
            require->push($2);
            $$ = dynamic_cast<sieve::ASTNode *>(require);
        }
    | IDENTIFIER arguments ";"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            command->push($2);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | IDENTIFIER ";"
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | FOREVERYPART block
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | FOREVERYPART TAG STRING_LITERAL block
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | if_flow
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch(@1);
            $$ = dynamic_cast<sieve::ASTNode *>(branch);
        }
    | if_flow ELSE block
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch(@1);
            $$ = dynamic_cast<sieve::ASTNode *>(branch);
        }
    | ";"
        {
            
        }
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
    | numeric { $$ = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(&$1)); }
    | TAG
        {
            // FIXME: fill in
        }
    ;

test_list : "(" tests ")" { $$ = $2; }
    ;

tests : test { $$ = $1; }
    | tests "," test { $1.insert($1.end(), $3.begin(), $3.end()); $$ = $1; }
    ;

test :
    IDENTIFIER arguments 
        {
            // FIXME: fill in
        }
    | TRUE { $$ = std::vector<sieve::ASTNode *>(1, dynamic_cast<sieve::ASTNode *>(&$1)); }
    | FALSE { $$ = std::vector<sieve::ASTNode *>(1, dynamic_cast<sieve::ASTNode *>(&$1)); }
    ;

string_list : STRING_LITERAL {$$ = std::vector<sieve::ASTNode *>(1, dynamic_cast<sieve::ASTNode *>(&$1)); }
    | "[" strings "]" { $$ = $2; }
    ;

strings : STRING_LITERAL {$$ = std::vector<sieve::ASTNode *>(1, dynamic_cast<sieve::ASTNode *>(&$1)); }
    | strings "," STRING_LITERAL { $1.push_back(&$3); $$ = $1; }
    ;

numeric : NUMBER { $$ = $1; }
    | NUMBER QUANTIFIER { $$ = $1; }
    ;

%%

void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.error(l, m);
}
