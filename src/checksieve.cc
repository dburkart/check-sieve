#include "checksieve.h"
#include "sieve_driver.hh"

namespace sieve
{

#define LIBCHECKSIEVE_VERSION "0.4-dev"

extern const char *version() {
    return LIBCHECKSIEVE_VERSION;
}

struct parse_result sieve_parse_file( const char *filename ) {
    driver driver;
    driver.parse_file(filename);
    return driver.result;
}

struct parse_result sieve_parse_string( const char *sieve ) {
    driver driver;
    driver.parse_string(sieve);
    return driver.result;
}

}
