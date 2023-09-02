%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines
%define api.parser.class {sieve_parser}

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
    @$.begin.filename = @$.end.filename = &driver.filepath;
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
%token <std::string> STRING_LITERAL
%token <int> NUMBER "number"
%token <bool> TRUE "true"
%token <bool> FALSE "false"

%type <sieve::ASTNumeric *> numeric
%type <std::vector<sieve::ASTNode *>> argument
%type <std::vector<sieve::ASTNode *>> arguments
%type <std::vector<sieve::ASTNode *>> test
%type <std::vector<sieve::ASTNode *>> tests
%type <std::vector<sieve::ASTNode *>> test_list

%type <std::vector<sieve::ASTNode *>> strings
%type <std::vector<sieve::ASTNode *>> string_list

%type <sieve::ASTNode *> command
%type <std::vector<sieve::ASTNode *>> command_list
%type <sieve::ASTNode *> block
%type <sieve::ASTNode *> if_flow

%type <sieve::ASTSieve *> sieve

%%
%start sieve;

sieve : END
        {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(@1);
            driver.syntax_tree(sieve);
            $$ = sieve;
        }
    | command_list
        {
            sieve::ASTSieve *sieve = new sieve::ASTSieve(@1);
            sieve->push($1);
            driver.syntax_tree(sieve);
            $$ = sieve;
        }
    ;

command_list : command { $$ = std::vector<sieve::ASTNode *>(1, $1); }
    | command_list command { $1.push_back($2); $$ = $1; }
    ;

command :
      REQUIRE string_list SEMICOLON
        {
            sieve::ASTRequire *require = new sieve::ASTRequire(@1);
            if ($2.size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(@2);
                stringList->push($2);
                require->push(stringList);
            } else {
                require->push($2);
            }
            $$ = dynamic_cast<sieve::ASTNode *>(require);
        }
    | IDENTIFIER arguments SEMICOLON
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            command->push($2);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | IDENTIFIER SEMICOLON
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | FOREVERYPART block
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            command->push($2);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | FOREVERYPART TAG STRING_LITERAL block
        {
            sieve::ASTCommand *command = new sieve::ASTCommand(@1, $1);
            command->push(new sieve::ASTTag(@2, $2));
            command->push(new sieve::ASTString(@3, $3));
            command->push($4);
            $$ = dynamic_cast<sieve::ASTNode *>(command);
        }
    | if_flow { $$ = $1; }
    | if_flow ELSE block
        {
            $1->push($3);
            $$ = $1;
        }
    ;

block : "{" command_list "}"
        {
            sieve::ASTBlock *block = new sieve::ASTBlock( @1 );
            block->push($2);
            $$ = dynamic_cast<sieve::ASTNode *>( block );
        }
    | "{" "}"
        {
            sieve::ASTNoOp *noop = new sieve::ASTNoOp( @1 );
            $$ = noop;
        }
    ;

if_flow : IF test block
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch( @1 );
            sieve::ASTCondition *condition = new sieve::ASTCondition( @2 );
            condition->push($2);
            branch->push(condition);
            branch->push($3);
            $$ = dynamic_cast<sieve::ASTNode *>(branch);

        }
    | IF "(" test ")" block
        {
            sieve::ASTBranch *branch = new sieve::ASTBranch( @1 );
            sieve::ASTCondition *condition = new sieve::ASTCondition( @3 );
            condition->push($3);
            branch->push(condition);
            branch->push($5);
            $$ = dynamic_cast<sieve::ASTNode *>(branch);

        }
    | if_flow ELSIF test block
        {
            sieve::ASTCondition *condition = new sieve::ASTCondition( @3 );
            condition->push($3);
            $1->push(condition);
            $1->push($4);
            $$ = $1;
        }
    ;

arguments : argument { $$ = $1; }
    | argument test { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | argument test_list { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | arguments argument { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
    | test { $$ = $1; }
    | test_list { $$ = $1; }
    ;

argument : string_list
        {
            if ($1.size() > 1) {
                sieve::ASTStringList *stringList = new sieve::ASTStringList(@1);
                stringList->push($1);
                $$ = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(stringList));
            } else {
                $$ = $1;
            }
        }
    | numeric
        {
            $$ = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>($1));
        }
    | TAG
        {
            sieve::ASTTag *tag = new sieve::ASTTag(@1, $1);
            $$ = std::vector<sieve::ASTNode *>( 1, dynamic_cast<sieve::ASTNode *>(tag));
        }
    ;

test_list : "(" tests ")" 
    {
        sieve::ASTTestList *testList = new sieve::ASTTestList( @1 );
        testList->push($2);
        $$ = std::vector<sieve::ASTNode *>(1, testList);
    }
    ;

tests : test { $$ = $1; }
    | tests "," test { $1.insert($1.end(), $3.begin(), $3.end()); $$ = $1; }
    ;

test :
    IDENTIFIER arguments
        {
            sieve::ASTTest *test = new sieve::ASTTest(@1, $1);
            test->push($2);
            $$ = std::vector<sieve::ASTNode *>(1, test);
        }
    | IDENTIFIER { $$ = std::vector<sieve::ASTNode *>(1, new sieve::ASTTest(@1, $1)); }
    | TRUE { $$ = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(@1, $1)); }
    | FALSE { $$ = std::vector<sieve::ASTNode *>(1, new sieve::ASTBoolean(@1, $1)); }
    ;

string_list : STRING_LITERAL {$$ = std::vector<sieve::ASTNode *>(1,  new sieve::ASTString(@1, $1)); }
    | "[" strings "]" { $$ = $2; }
    ;

strings : STRING_LITERAL {$$ = std::vector<sieve::ASTNode *>(1, new sieve::ASTString(@1, $1)); }
    | strings "," STRING_LITERAL { $1.push_back( new sieve::ASTString(@3, $3)); $$ = $1; }
    ;

numeric :
    NUMBER
        {
            $$ = new sieve::ASTNumeric(@1, $1);
        }
    | NUMBER QUANTIFIER
        {
            // TODO: Somehow incorporate the quantifier in here
            $$ = new sieve::ASTNumeric(@1, $1);
        }
    ;

%%

void yy::sieve_parser::error( const location_type &l, const std::string &m ) {
    driver.push_error(l, m);
}
