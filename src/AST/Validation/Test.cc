#include <string>

#include "ASTString.hh"
#include "ASTStringList.hh"
#include "ASTTag.hh"
#include "checksieve.h"
#include "Test.hh"

namespace sieve {

bool validateBareTest(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    size_t size = test->children().size();

    if (size == 0)
        return true;
    else
        return false;
}

bool validateExists(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    size_t size = test->children().size();

    if (size < 1 || size > 3)
        return false;

    const ASTString *stringChild = dynamic_cast<const ASTString*>(test->children()[size - 1]);
    const ASTStringList *stringListChild = dynamic_cast<const ASTStringList*>(test->children()[size - 1]);
    if (stringChild == NULL && stringListChild == NULL)
        return false;

    // If we have more than 1 child, check that they are either :mime or :anychild
    if (size > 1) {
        const ASTTag *firstTag = dynamic_cast<const ASTTag*>(test->children()[0]);
        if (firstTag == NULL)
            return false;

        if (firstTag->value() != ":mime" && firstTag->value() != ":anychild")
            return false;
    }

    if (size > 2) {
        const ASTTag *secondTag = dynamic_cast<const ASTTag*>(test->children()[1]);
        if (secondTag == NULL)
            return false;

        if (secondTag->value() != ":mime" && secondTag->value() != ":anychild")
            return false;
    }

    return true;
}

bool validateHasOnlyTestList(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    std::vector<sieve::ASTNode *> children = test->children();

    for (std::vector<ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        const ASTTest *child = dynamic_cast<ASTTest*>(*it);

        if (child == NULL)
            return false;
    }

    return true;
}

bool validateNotTest(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    size_t size = test->children().size();

    if (size != 1)
        return false;

    return true;
}

bool validateSizeTest(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    size_t size = test->children().size();

    if (size != 2)
        return false;

    const ASTTag *tag = dynamic_cast<const ASTTag*>(test->children()[0]);
    if (tag == NULL || (tag->value() != ":over" && tag->value() != ":under"))
        return false;

    return true;
}

bool validateValidNotifyMethodTest(const ASTNode *node) {
    const ASTTest *test = dynamic_cast<const ASTTest*>(node);
    size_t size = test->children().size();

    if (size != 1)
        return false;

    if (!nodeIsType<ASTStringList>(test->children()[0]) && !nodeIsType<ASTString>(test->children()[0]))
        return false;

    return true;
}

Test::Test() {
    _usage_map["allof"]                 = "allof <tests: test-list>";
    _usage_map["anyof"]                 = "anyof <tests: test-list>";
    _usage_map["exists"]                = "exists [:mime] [:anychild] <header-names: string-list>";
    _usage_map["not"]                   = "not <test1: test>";
    _usage_map["size"]                  = "size <:over / :under> <limit: number>";
    _usage_map["valid_notify_method"]   = "valid_notify_method <notification-uris: string-list>";

    _validation_fn_map["allof"]                 = &validateHasOnlyTestList;
    _validation_fn_map["anyof"]                 = &validateHasOnlyTestList;
    _validation_fn_map["exists"]                = &validateExists;
    _validation_fn_map["not"]                   = &validateNotTest;
    _validation_fn_map["size"]                  = &validateSizeTest;
    _validation_fn_map["valid_notify_method"]   = &validateValidNotifyMethodTest;
}

bool Test::validate(const ASTTest *test) {
    if (!_validation_fn_map[test->value()]) {
        DEBUGLOG(test->value(), "Test is missing validation.")
        return true;
    }

    return _validation_fn_map[test->value()](test);
}

std::string Test::usage(const ASTTest *test) {
    return "Usage: " + _usage_map[test->value()];
}

}