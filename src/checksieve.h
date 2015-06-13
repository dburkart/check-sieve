#ifndef __CHECKSIEVE_H__
#define __CHECKSIEVE_H__

#include <string>
#include "location.hh"

struct parse_result {
    int status;
    yy::location location;
    std::string error;
    
    parse_result() {
        status = 0;
    }
};

struct parse_result sieve_parse_file( const char *filename );

#endif