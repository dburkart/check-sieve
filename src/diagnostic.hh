#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include <string>

#include "checksieve.h"

namespace sieve {

class Diagnostic {
public:
	std::string describe(const parse_result &result, std::istream &input);
};

} // namespace sieve

#endif
