#include "diagnostic.hh"

#include <sstream>
#include <string>

namespace sieve {

std::string Diagnostic::describe( parse_result result, std::ifstream &input ) {
    std::string line;
    std::ostringstream output;

    for (int i = 1; getline(input, line, '\n'); i++) {
        if (i == result.location.begin.line)
            break;
    }

    // Account for tabs in our input
    for (int i = 0; i < result.location.begin.column; i++) {
        if (line[i] == '\t') {
            line.replace(i, 1, std::string(_tab_size, ' '));
            result.location.begin.column += _tab_size - 1;
            result.location.end.column += _tab_size - 1;
        }
    }

    output << result.error << std::endl
           << "On line " << result.location.begin.line << ":" << std::endl
           << line << std::endl
           << std::string(result.location.begin.column - 1, ' ')
           << std::string(result.location.end.column - result.location.begin.column, '^')
           << std::endl;

    if (result.hint != "") {
        output << result.hint << std::endl;
    }

    return output.str();
}

}