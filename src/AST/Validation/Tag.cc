#include "ASTCommand.hh"
#include "ASTNumeric.hh"
#include "ASTString.hh"
#include "ASTStringList.hh"
#include "ASTTag.hh"
#include "Tag.hh"
#include "ASTTest.hh"
#include "ASTVerificationVisitor.hh"

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

    // RFC 5260
    _usage_map[":zone"] = ":zone <time-zone: string>";

    // RFC 6009
    _usage_map[":bytimerelative"] = ":bytimerelative <rlimit: number>";
    _usage_map[":bytimeabsolute"] = ":bytimeabsolute <alimit: string>";
    _usage_map[":bymode"] = ":bymode \"notify\" | \"return\"";

    // RFC 7352
    _usage_map[":handle"] = ":handle string";
    _usage_map[":header"] = ":header string";
    _usage_map[":uniqueid"] = ":uniqueid string";
    _usage_map[":seconds"] = ":seconds string";
    _usage_map[":specialuse"] = "fileinto [:specialuse string] ...";

    // RFC 9042
    _usage_map[":mailboxid"] = "fileinto [:mailboxid string] ...";

    // RFC 9671
    _usage_map[":organizers"] = ":organizers <ext-list-name: string>";
    _usage_map[":outcome"] = ":outcome <variable-name: string>";
    _usage_map[":reason"] = ":reason <variable-name: string>";
    _usage_map[":addresses"] = ":addresses <string-list>";
    _usage_map[":calendarid"] = ":calendarid <string>";

    _usage_map[":input"] = "execute [:input string] ...";
    _usage_map[":output"] = "execute [:output varname] ...";

    _validation_fn_map[":handle"] = &Tag::_validateSingleString;
    _validation_fn_map[":header"] = &Tag::_validateSingleString;
    _validation_fn_map[":uniqueid"] = &Tag::_validateSingleString;
    _validation_fn_map[":seconds"] = &Tag::_validateSingleNumeric;
    _validation_fn_map[":bytimerelative"] = &Tag::_validateSingleNumeric;
    _validation_fn_map[":bytimeabsolute"] = &Tag::_validateSingleString;
    _validation_fn_map[":bymode"] = &Tag::_validateByMode;
    _validation_fn_map[":specialuse"] = &Tag::_validateSpecialUse;
    _validation_fn_map[":mailboxid"] = &Tag::_validateSpecialUse;
    _validation_fn_map[":input"] = &Tag::_validateSingleString;
    _validation_fn_map[":output"] = &Tag::_validateSingleString;
    _validation_fn_map[":zone"] = &Tag::_validateZone;
    _validation_fn_map[":organizers"] = &Tag::_validateOrganizers;
    _validation_fn_map[":outcome"] = &Tag::_validateOutcomeOrReason;
    _validation_fn_map[":reason"] = &Tag::_validateOutcomeOrReason;
    _validation_fn_map[":addresses"] = &Tag::_validateList;
    _validation_fn_map[":calendarid"] = &Tag::_validateSingleString;
}

ValidationResult Tag::validate(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    
    if (!_validation_fn_map[tag->value()]) {
        DEBUGLOG(tag->value() + " tag is missing validation.")
        return ValidationResult(true);
    }

    return (this->*_validation_fn_map[tag->value()])(tag);
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
    const auto *parent = dynamic_cast<const ASTCommand*>(tag->parent());

    if (parent->value() != "fileinto") {
        return {false, "\"" + tag->value() + "\" is only valid as part of the fileinto command"};
    }

    if (tag->value() == ":mailboxid") {
        if (parent->find(ASTTag(":mailboxid")) != parent->children().end() && parent->find(ASTTag(":specialuse")) != parent->children().end()) {
            return {false, ":specialuse is disallowed when using :mailboxid, choose one or the other.", true};
        }
    }

    return ValidationResult(true);
}

ValidationResult Tag::_validateZone(const ASTNode *node) {
    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const auto *parent = dynamic_cast<const ASTTest*>(tag->parent());
    const ASTNode *next = parent->nextChild(tag);

    if (dynamic_cast<const ASTString*>(next) == nullptr)
        return ValidationResult(false);

    if (parent->value() == "envelope") {
        auto v = dynamic_cast<const ASTVerificationVisitor*>(this->visitor());
        if (v != nullptr && !v->has_required("envelope-deliverby")) {
            return ValidationResult(false, "Using :zone with envelope requires the envelope-deliverby extension.", true);
        }
    }

    return ValidationResult(true);
}

ValidationResult Tag::_validateByMode(const ASTNode *node) {
    auto result = _validateSingleString(node);
    if (!result.result()) {
        return result;
    }

    const auto *tag = dynamic_cast<const ASTTag*>(node);
    const auto *next = dynamic_cast<const ASTString*>(tag->parent()->nextChild(tag));

    if (next == nullptr)
        return ValidationResult(false);

    if (next->value() != "notify" && next->value() != "return")
        return ValidationResult(false, "Unexpected argument to :bymode", true);

    return ValidationResult(true);
}

ValidationResult Tag::_validateOrganizers(const ASTNode *node) {
    auto result = _validateSingleString(node);
    if (!result.result()) {
        return result;
    }

    auto v = dynamic_cast<const ASTVerificationVisitor*>(this->visitor());
    if (v != nullptr && !v->has_required("extlists")) {
        return ValidationResult(false, "The :organizers tag requires the extlists extension.", true);
    }

    return ValidationResult(true);
}

ValidationResult Tag::_validateOutcomeOrReason(const ASTNode *node) {
    auto result = _validateSingleString(node);
    if (!result.result()) {
        return result;
    }

    const auto *tag = dynamic_cast<const ASTTag*>(node);

    auto v = dynamic_cast<const ASTVerificationVisitor*>(this->visitor());
    if (v != nullptr && !v->has_required("variables")) {
        return ValidationResult(false, "The " + tag->value() + " tag requires the variables extension.", true);
    }

    return ValidationResult(true);

}

}