#include "Tag.hh"

namespace sieve {

Tag::Tag() {
    
}

bool Tag::validate(const ASTTag *tag) {
    if (!_validation_fn_map[tag->value()]) {
        DEBUGLOG(tag->value() + " tag is missing validation.")
        return true;
    }

    return _validation_fn_map[tag->value()](tag);
}

std::string Tag::usage(const ASTTag *tag) {
    return "Usage: " + _usage_map[tag->value()];
}
}