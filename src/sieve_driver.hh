#ifndef __SIEVE_DRIVER_HH__
#define __SIEVE_DRIVER_HH__

#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "sieve_parser.tab.hh"
#include "checksieve.h"
#include "AST.hh"

#define YY_DECL \
    yy::sieve_parser::symbol_type yylex( yyscan_t yyscanner, sieve::driver &driver )
YY_DECL;

namespace sieve
{

class driver {
public:
    driver();
    driver(struct parse_options options);
    virtual ~driver();
    
    void scan_begin();
    void scan_begin( const std::string &ifstream);
    void scan_end();
    bool trace_scanning;
    
    parse_result parse_file( const std::string &fp );
    parse_result parse_string( const std::string &sieve );

    std::string filepath;
    std::string sieve_string;
    bool trace_parsing;
    bool trace_tree;

    int result;

    void push_error( const yy::location &l, const std::string &message );
    
    ASTSieve *syntax_tree() { return _sieve; }
    ASTSieve *syntax_tree(ASTSieve *sieve) { _sieve = sieve; return _sieve; }

    struct parse_options parse_options() { return _options; }

private:
    struct parse_options _options;
    std::istringstream _input_stream;
    yyscan_t yyscanner{};
    
    ASTSieve *_sieve{};

    std::vector<parse_result> _errors;
};

} // namespace sieve

#endif
