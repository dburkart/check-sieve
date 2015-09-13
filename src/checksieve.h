#ifndef __CHECKSIEVE_H__
#define __CHECKSIEVE_H__

#include <string>
#include "location.hh"

namespace sieve 
{

struct parse_result {
    int status;
    yy::location location;
    std::string error;
    
    parse_result() {
        status = 0;
    }

    void set_error(yy::location loc, std::string err) {
        status = 1;
        loc = location;
        error = err;
    }
};

struct parse_result sieve_parse_file( const char *filename );
struct parse_result sieve_parse_string( const char *sieve );

const char *version();

} // namespace sieve

#endif