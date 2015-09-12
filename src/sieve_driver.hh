#ifndef __SIEVE_DRIVER_HH__
#define __SIEVE_DRIVER_HH__

#include <vector>
#include <string>
#include <sstream>
#include <map>
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
    driver(bool quiet);
    virtual ~driver();
    
    void scan_begin();
    void scan_begin( const std::string &sieve);
    void scan_end();
    bool trace_scanning;
    
    int parse_file( const std::string &f );
    int parse_string( const std::string &sieve );
    std::string file;
    std::string sieve_string;
    bool trace_parsing;
    
    struct parse_result result;
    
    void add_required_modules(std::vector<std::string> &modules);
    bool supports_module(const std::string &mod);
    
    void init_maps();
    bool valid_command(const std::string &command);
    bool valid_test(const std::string &command);
    
    void error( const yy::location &l, const std::string &message, const std::string &suggestion);
    void error( const yy::location &l, const std::string &message );
    void error( const std::string &m ); 

private:
    std::map<std::string, bool> _command_map;
    std::map<std::string, bool> _test_map;
    
    std::vector<std::string> _modules;
    
    std::istringstream _input_stream;
    yyscan_t yyscanner;
    bool _suppress_output;
};

} // namespace sieve

#endif
