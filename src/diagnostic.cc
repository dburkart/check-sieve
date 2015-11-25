#include "diagnostic.hh"

#include <sstream>
#include <string>

namespace sieve {

std::string Diagnostic::describe( const parse_result &result, std::istream &input ) {
    std::string line;
    std::ostringstream output;

    for (int i = 1; getline(input, line, '\n'); i++) {
        if (i == result.location.begin.line)
            break;
    }

    output << result.error << std::endl;
    output << "On line " << result.location.begin.line << ":" << std::endl;
    output << line << std::endl;
    output << std::string(result.location.begin.column - 1, ' ')
    		<< std::string(result.location.end.column - result.location.begin.column, '^')
    		<< std::endl;

    return output.str();
}

}