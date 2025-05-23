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
    ValidationResult _validateSingleString(const ASTNode *node);
    ValidationResult _validateSingleNumeric(const ASTNode *node);
    ValidationResult _validateList(const ASTNode *node);
    ValidationResult _validateSpecialUse(const ASTNode *node);
    ValidationResult _validateZone(const ASTNode *node);
    ValidationResult _validateByMode(const ASTNode *node);
    ValidationResult _validateOrganizers(const ASTNode *node);
    ValidationResult _validateOutcomeOrReason(const ASTNode *node);

    std::map<std::string, ValidationResult (Tag::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
