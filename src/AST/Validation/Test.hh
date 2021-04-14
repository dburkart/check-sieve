#ifndef __TEST_H__
#define __TEST_H__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTTest.hh"

namespace sieve 
{

class Test : public Validator {
public:
    Test();
    ~Test() {}

    bool validate(const ASTNode *command);
    std::string usage(const ASTNode *command);

private:
    // Validation functions
    bool _validateExists(const ASTNode *node);
    bool _validateHasOnlyTestList(const ASTNode *node);
    bool _validateNotTest(const ASTNode *node);
    bool _validateSizeTest(const ASTNode *node);
    bool _validateValidNotifyMethodTest(const ASTNode *node);
    bool _validateHeaderTest(const ASTNode *node);
    bool _validateIhaveTest(const ASTNode *node);
    bool _validateEnvironmentTest (const ASTNode *node);

    std::map<std::string, bool (Test::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif