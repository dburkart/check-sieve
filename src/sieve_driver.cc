#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

#include "sieve_driver.hh"
#include "sieve_parser.tab.hh"
#include "AST.hh"

namespace sieve
{

driver::driver()
    : trace_scanning( false )
    , trace_parsing( false )
    , trace_tree( false )
    , _options()
    , result( 0 )
    , yyscanner()
    , _sieve() {}

driver::driver( struct parse_options options )
    : trace_scanning( false )
    , trace_parsing( false )
    , trace_tree( false )
    , _options( options )
    , result( 0 )
    , yyscanner()
    , _sieve() {}

driver::~driver() = default;

parse_result driver::parse_file( const std::string &fp ) {
    std::string line;
    std::string buffer;
    std::ifstream file( fp );

    filepath = fp;

    for (int i = 1; !file.eof(); i++) {
        getline(file, line);
        buffer += line + "\n";
    }

    file.seekg(0, std::ifstream::beg);

    return parse_string(buffer);
}

parse_result driver::parse_string( const std::string &sieve ) {
    parse_result res;

    res.status = 0;
    sieve_string = sieve + "\n";
    scan_begin( sieve_string );
    yy::sieve_parser parser( yyscanner, *this );
    parser.set_debug_level( trace_parsing );
    parser.parse();
    scan_end();

    if ( result )
        res = _errors[0];

    if ( !result ) {
        ASTVerificationVisitor visitor = ASTVerificationVisitor( _options );
        visitor.walk(syntax_tree());

        res = visitor.result();
        result = res.status;
    }

    return res;
}

void driver::push_error( const yy::location &l, const std::string &message ) {
    result = 1;
    _errors.push_back({ 1, l, message });
}

} // namespace sieve
