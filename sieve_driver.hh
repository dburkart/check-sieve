#ifndef __SIEVE_DRIVER_HH__
#define __SIEVE_DRIVER_HH__

#include <vector>
#include <string>
#include <map>
#include "sieve_parser.tab.hh"

#define YY_DECL \
    yy::sieve_parser::symbol_type yylex( sieve_driver &driver )

YY_DECL;

class sieve_driver {
public:
    sieve_driver();
    virtual ~sieve_driver();
    
    void scan_begin();
    void scan_end();
    bool trace_scanning;
    
    int parse( const std::string &f );
    std::string file;
    bool trace_parsing;
    
    int result;
    
    void set_required_modules(std::vector<std::string> &modules);
    bool supports_module(const std::string &mod);
    
    void error( const yy::location &l, const std::string &message, const std::string &suggestion);
    void error( const yy::location &l, const std::string &message );
    void error( const std::string &m ); 

private:
    std::map<std::string, bool> _module_map;
};

#endif
