#include "ASTString.hh"
#include "Tag.hh"

namespace sieve {

bool validateComparator(const ASTNode *node) {
    const ASTTag *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);

    if (next == NULL) {
        return false;
    }

    const ASTString *nextString = dynamic_cast<const ASTString *>(next) ;
    if (nextString == NULL) {
        return false;
    }

    // Validate comparator string

    return true;
}

Tag::Tag() {
    _usage_map[":comparator"] = ":comparator string";

    _validation_fn_map[":comparator"] = &validateComparator;
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