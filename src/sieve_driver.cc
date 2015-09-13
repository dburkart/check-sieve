#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

#include "sieve_driver.hh"
#include "sieve_parser.tab.hh"
#include "AST.hh"

namespace sieve
{

driver::driver()
    : trace_scanning( false )
    , trace_parsing( false )
    , trace_tree( false )
    , _suppress_output(false) {}

driver::driver( bool quiet )
    : trace_scanning( false )
    , trace_parsing( false )
    , trace_tree( false )
    , _suppress_output( quiet ) {}

driver::~driver() {}

int driver::parse_file( const std::string &f ) {
    file = f;
    std::ifstream fin( file.c_str() );
    std::string line;
    std::string buffer;

    for (int i = 1; !fin.eof(); i++) {
        getline(fin, line);
        buffer += line + "\n";
    }

    return parse_string(buffer);
}

int driver::parse_string( const std::string &sieve ) {
    sieve_string = sieve + "\n";
    scan_begin( sieve_string );
    yy::sieve_parser parser( yyscanner, *this );
    parser.set_debug_level( trace_parsing );
    result.status = parser.parse();
    scan_end();

    if (!result.status) {
        ASTVerificationVisitor visitor = ASTVerificationVisitor();
        visitor.walk(syntax_tree());
        parse_result res = visitor.result();

        if (res.status)
            error(res.location, res.error);

        result.status = res.status;
    }

    return result.status;
}

void driver::error( const yy::location &l, const std::string &message, const std::string &suggestion ) {
    error( l, message );

    if (!_suppress_output) {
        std::cerr << std::endl << suggestion << std::endl;
    }
}

void driver::error( const yy::location &l, const std::string &message ) {
    std::istringstream f( sieve_string );
    std::string line;

    for (int i = 1; getline(f, line, '\n'); i++) {
        if (i == l.begin.line)
            break;
    }

    result.location = l;
    result.error = message;

    if (!_suppress_output) {
        std::cerr << message << std::endl;
        std::cerr << "On line " << l.begin.line << ":" << std::endl;
        std::cerr << line << std::endl;
        std::cerr << std::string(l.begin.column - 1, ' ') << std::string(l.end.column - l.begin.column, '^') << std::endl;
    }
}

void driver::error( const std::string &m ) {
    std::cerr << m << std::endl;
}

void driver::error( const parse_result result ) {
    error( result.location, result.error );
}

} // namespace sieve
