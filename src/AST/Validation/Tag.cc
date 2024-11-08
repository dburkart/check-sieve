#include "ASTCommand.hh"
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
    _usage_map[":fcc"] = ":fcc <mailbox: string>";

    _validation_fn_map[":comparator"] = &Tag::_validateSingleString; // TODO: Validate comparator string
    _validation_fn_map[":days"] = &Tag::_validateSingleNumeric;
    _validation_fn_map[":subject"] = &Tag::_validateSingleString;
    _validation_fn_map[":eval"] = &Tag::_validateSingleString;
    _validation_fn_map[":list"] = &Tag::_validateList;
    _validation_fn_map[":fcc"] = &Tag::_validateSingleString;

    // RFC 7352
    _usage_map[":handle"] = ":handle string";
    _usage_map[":header"] = ":header string";
    _usage_map[":uniqueid"] = ":uniqueid string";
    _usage_map[":seconds"] = ":seconds string";
    _usage_map[":specialuse"] = "fileinto [:specialuse string] ...";

    _validation_fn_map[":handle"] = &Tag::_validateSingleString;
    _validation_fn_map[":header"] = &Tag::_validateSingleString;
    _validation_fn_map[":uniqueid"] = &Tag::_validateSingleString;
    _validation_fn_map[":seconds"] = &Tag::_validateSingleNumeric;
    _validation_fn_map[":specialuse"] = &Tag::_validateSpecialUse;
}

ValidationResult Tag::validate(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    
    if (!_validation_fn_map[tag->value()]) {
        DEBUGLOG(tag->value() + " tag is missing validation.")
        return ValidationResult(true);
    }

    return (this->_validation_fn_map[tag->value()])(tag);
}

std::string Tag::usage(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    return "Usage: " + _usage_map[tag->value()];
}

//-- Private members
ValidationResult Tag::_validateSingleString(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);
    
    if (next == nullptr) {
        return ValidationResult(false);
    }
    
    const auto *nextString = dynamic_cast<const ASTString *>(next) ;
    if (nextString == nullptr) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Tag::_validateSingleNumeric(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);
    
    if (next == nullptr) {
        return ValidationResult(false);
    }
    
    const auto *nextString = dynamic_cast<const ASTNumeric *>(next) ;
    if (nextString == nullptr) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Tag::_validateList(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTNode *parent = tag->parent();
    const ASTNode *next = parent->nextChild(tag);

    if (dynamic_cast<const ASTString *>(next) != nullptr) {
        return ValidationResult(true);
    }

    if (dynamic_cast<const ASTStringList *>(next) != nullptr) {
        return ValidationResult(true);
    }

    return ValidationResult(false);
}

ValidationResult Tag::_validateSpecialUse(const ASTNode *node) {
    auto result = _validateSingleString(node);
    if (!result.result()) {
        return result;
    }

    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const ASTCommand *parent = dynamic_cast<const ASTCommand*>(tag->parent());

    if (parent->value() != "fileinto") {
        return ValidationResult(false, "\":specialuse\" is only valid as part of the fileinto command");
    }

    return ValidationResult(true);
}

}