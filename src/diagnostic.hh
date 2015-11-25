#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include <fstream>
#include <string>

#include "checksieve.h"

namespace sieve {

class Diagnostic {
public:
	std::string describe(parse_result result, std::ifstream &input);

private:
	size_t _tab_size = 4;
};

} // namespace sieve

#endif
