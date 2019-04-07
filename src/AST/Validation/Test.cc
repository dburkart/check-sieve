#include "Test.hh"

namespace sieve {

Test::Test() {
    // TODO: Set up usage and validation map here
}

bool Test::validate(const ASTTest *test) {
    if (!_validation_fn_map[test->value()])
        return true;

    return _validation_fn_map[test->value()](test);
}

std::string Test::usage(const ASTTest *test) {
    return "Usage: " + _usage_map[test->value()];
}

}