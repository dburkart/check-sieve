#ifndef __CHECKSIEVE_H__
#define __CHECKSIEVE_H__

#ifndef PLATFORM
#if defined __APPLE__ && defined __MACH__
#define PLATFORM "Darwin"
#endif
#endif

#include <string>
#include "location.hh"

namespace sieve 
{

struct parse_result {
    int status;
    yy::location location;
    std::string error;
    std::string hint;

    void set_error( std::string err ) {
        status = 1;
        error = err;
    }

    void set_error( std::string err, yy::location loc ) {
        status = 1;
        error = err;
        location = loc;
    }
};

struct parse_result sieve_parse_file( const char *filename );
struct parse_result sieve_parse_string( const char *sieve );

const char *version();

} // namespace sieve

#endif
