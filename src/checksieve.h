#ifndef __CHECKSIEVE_H__
#define __CHECKSIEVE_H__

#include <string>
#include "sieve_driver.hh"

namespace yy {
    class location;
}

struct parse_result {
    int status;
    yy::location location;
    std::string error;
};

struct parse_result sieve_parse_file( const char *filename );

#endif