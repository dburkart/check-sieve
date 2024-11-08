#pragma once

#include <map>
#include <string>

#include "Validation.hh"

namespace sieve 
{

class Tag : public Validator{
public:
    Tag();
    ~Tag() = default;

    ValidationResult validate(const ASTNode *node) override;
    std::string usage(const ASTNode *node) override;

private:
    // Validation functions
    static ValidationResult _validateSingleString(const ASTNode *node);
    static ValidationResult _validateSingleNumeric(const ASTNode *node);
    static ValidationResult _validateList(const ASTNode *node);
    static ValidationResult _validateSpecialUse(const ASTNode *node);
    
    std::map<std::string, ValidationResult (*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
