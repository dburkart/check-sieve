%token NUMBER "number"
%token STRING_LITERAL "string"
%token QUANTIFIER "quantifier"
%token SIZE_TAG TAG "tag"

%token IDENTIFIER IDENTIFIER_SINGLE_STRING IDENTIFIER_NO_STRINGS REQUIRE "identifier"
%token FLAG_COMMAND HASFLAG_COMMAND
%token ADDRESS ALLOF ANYOF ENVELOPE EXISTS TRUE FALSE HEADER NOT SIZE
%token IF ELSIF ELSE

%%

commands : command
        | commands command
        ;

command : IDENTIFIER ';'
        | IDENTIFIER arguments ';'
        | IDENTIFIER arguments block
        | IF test block
        | IF test block ELSIF test block
        | IF test block ELSIF test block ELSE block
        | REQUIRE string_list ';'
        | IDENTIFIER_NO_STRINGS ';'
        | IDENTIFIER_SINGLE_STRING STRING_LITERAL ';'
        | FLAG_COMMAND string_list
        | FLAG_COMMAND STRING_LITERAL string_list
        | HASFLAG_COMMAND string_list
        | HASFLAG_COMMAND string_list string_list
        | HASFLAG_COMMAND double_tag string_list
        | HASFLAG_COMMAND double_tag string_list string_list
        ;

block   : '{' commands '}'
        ;

argument : string_list
        | numeric
        | TAG
        ;

arguments : argument
        | argument test
        | argument test_list
        | arguments argument
        ;

double_tag : TAG
        | TAG TAG
        ;

triple_tag : double_tag
        | TAG TAG TAG
        ;


test : ADDRESS string_list string_list
     | ADDRESS triple_tag string_list string_list
     | ALLOF test_list
     | ANYOF test_list
     | ENVELOPE string_list string_list
     | ENVELOPE triple_tag string_list string_list
     | EXISTS string_list
     | TRUE
     | FALSE
     | HEADER string_list string_list
     | HEADER double_tag string_list string_list
     | NOT test
     | SIZE SIZE_TAG numeric
     ;

test_array : test
        | test_array ',' test
        ;

test_list : '(' test_array ')'
        ;

string_list : STRING_LITERAL
        | '[' string_array ']'
        ;

string_array: STRING_LITERAL
        | string_array ',' STRING_LITERAL
        ;

numeric : NUMBER
        | NUMBER QUANTIFIER
        ;

%%

int main (void) {return yyparse ( );}
