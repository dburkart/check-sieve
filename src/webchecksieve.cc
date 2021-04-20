#include <emscripten/bind.h>

#include "checksieve.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(sieve) {

    value_object<yy::position>("position")
        .field("column", &yy::position::column)
        .field("line", &yy::position::line)
        ;
    value_object<yy::location>("location")
        .field("begin", &yy::location::begin)
        .field("end", &yy::location::end)
        ;
    value_object<sieve::parse_result>("parse_result")
        .field("location", &sieve::parse_result::location)
        .field("status", &sieve::parse_result::status)
        .field("error", &sieve::parse_result::error)
        .field("hint", &sieve::parse_result::hint)
        ;
    class_<sieve::parse_options>("parse_options")
        .constructor<>()
        ;

    function("version", optional_override(
                [](){
                  return std::string(sieve::version());
                }));
    function("sieve_parse_string", optional_override(
                [](const std::string s, struct sieve::parse_options o) {
                  return sieve_parse_string(s.c_str(), o);
                }));
}
