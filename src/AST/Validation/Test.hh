#pragma once

#include <map>
#include <string>

#include "Validation.hh"

namespace sieve 
{

class Test : public Validator {
public:
    Test();
    ~Test() = default;

    ValidationResult validate(const ASTNode *command) override;
    std::string usage(const ASTNode *command) override;

private:
    // Validation functions
    ValidationResult _validateExists(const ASTNode *node);
    ValidationResult _validateHasOnlyTestList(const ASTNode *node);
    ValidationResult _validateHasOnlyStringList(const ASTNode *node);
    ValidationResult _validateNotTest(const ASTNode *node);
    ValidationResult _validateSizeTest(const ASTNode *node);
    ValidationResult _validateValidNotifyMethodTest(const ASTNode *node);
    ValidationResult _validateHasExpirationTest(const ASTNode *node);
    ValidationResult _validateExpirationTest(const ASTNode *node);
    ValidationResult _validateHeaderTest(const ASTNode *node);
    ValidationResult _validateIhaveTest(const ASTNode *node);
    ValidationResult _validateEnvironmentTest(const ASTNode *node);
    ValidationResult _validateDuplicateTest(const ASTNode *node);
    ValidationResult _validateSpecialUseExistsTest(const ASTNode *node);
    ValidationResult _validateFilterTest(const ASTNode *node);
    ValidationResult _validateExecuteTest(const ASTNode *node);

    std::map<std::string, ValidationResult (Test::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}