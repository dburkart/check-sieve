#pragma once

#include <fstream>
#include <string>

#include "checksieve.h"

namespace sieve {

class Diagnostic {
public:
	Diagnostic() : _tab_size( 4 ) { }
	explicit Diagnostic(size_t tab_size) : _tab_size( tab_size ) { }

	std::string describe(parse_result& result, std::ifstream &input) const;

private:
	size_t _tab_size;
};

} // namespace sieve

