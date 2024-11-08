#pragma once

#include <map>
#include <string>

#include "Validation.hh"

namespace sieve 
{

class Command : public Validator {
public:
    Command();
    ~Command() = default;

    ValidationResult validate(const ASTNode *node) override;
    std::string usage(const ASTNode *node) override;

private:
    // Validation functions
    static ValidationResult _validateAddHeadersCommand(const ASTNode *node);
    static ValidationResult _validateDeleteHeadersCommand(const ASTNode *node);
    static ValidationResult _validateIncludeCommand(const ASTNode *node);
    static ValidationResult _validateIMAP4FlagsAction(const ASTNode *node);
    static ValidationResult _validateFileintoCommand(const ASTNode *node);
    static ValidationResult _validateKeepCommand(const ASTNode *node);
    static ValidationResult _validateReplaceCommand(const ASTNode *node);
    static ValidationResult _validateEncloseCommand(const ASTNode *node);
    static ValidationResult _validateRedirectCommand(const ASTNode *node);
    static ValidationResult _validateSetCommand(const ASTNode *node);
    static ValidationResult _validateVacationCommand(const ASTNode *node);
    static ValidationResult _validateBareCommand(const ASTNode *node);
    static ValidationResult _validateSingleArgumentCommand(const ASTNode *node);
    static ValidationResult _validateSingleStringArgumentCommand(const ASTNode *node);
    static ValidationResult _validateBreakCommand(const ASTNode *node);
    static ValidationResult _validateForeverypartCommand(const ASTNode *node);
    static ValidationResult _validateExtracttextCommand(const ASTNode *node);
    static ValidationResult _validateExpireCommand(const ASTNode *node);
    static ValidationResult _validateNotifyCommand(const ASTNode *node);
    static ValidationResult _validateConvertCommand(const ASTNode *node);

    std::map<std::string, ValidationResult (*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
