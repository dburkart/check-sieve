#include "ASTNode.hh"
#include "ASTNumeric.hh"
#include "ASTString.hh"
#include "ASTStringList.hh"
#include "ASTTag.hh"
#include "Tag.hh"

namespace sieve {

Tag::Tag() {
    _usage_map[":comparator"] = ":comparator string";
    _usage_map[":days"] = ":days number";
    _usage_map[":subject"] = ":subject string";
    _usage_map[":eval"] = ":eval string";
    _usage_map[":list"] = ":list string";

    _validation_fn_map[":comparator"] = &Tag::_validateSingleString; // TODO: Validate comparator string
    _validation_fn_map[":days"] = &Tag::_validateSingleNumeric;
    _validation_fn_map[":subject"] = &Tag::_validateSingleString;
    _validation_fn_map[":eval"] = &Tag::_validateSingleString;
    _validation_fn_map[":list"] = &Tag::_validateList;
}

bool Tag::validate(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    
    if (!_validation_fn_map[tag->value()]) {
        DEBUGLOG(tag->value() + " tag is missing validation.")
        return true;
    }

    return (this->*_validation_fn_map[tag->value()])(tag);
}

std::string Tag::usage(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    return "Usage: " + _usage_map[tag->value()];
}

//-- Private members
bool Tag::_validateSingleString(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);
    
    if (next == nullptr) {
        return false;
    }
    
    const auto *nextString = dynamic_cast<const ASTString *>(next) ;
    if (nextString == nullptr) {
        return false;
    }
    
    return true;
}

bool Tag::_validateSingleNumeric(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);
    
    if (next == nullptr) {
        return false;
    }
    
    const auto *nextString = dynamic_cast<const ASTNumeric *>(next) ;
    if (nextString == nullptr) {
        return false;
    }
    
    return true;
}

bool Tag::_validateList(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);

    if (dynamic_cast<const ASTString *>(next) != nullptr) {
        return true;
    }

    if (dynamic_cast<const ASTStringList *>(next) != nullptr) {
        return true;
    }

    return false;
}

}