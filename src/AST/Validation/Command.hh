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

    bool validate(const ASTNode *node) override;
    std::string usage(const ASTNode *node) override;

private:
    // Validation functions
    static bool _validateAddHeadersCommand(const ASTNode *node);
    static bool _validateDeleteHeadersCommand(const ASTNode *node);
    static bool _validateIncludeCommand(const ASTNode *node);
    static bool _validateIMAP4FlagsAction(const ASTNode *node);
    static bool _validateFileintoCommand(const ASTNode *node);
    static bool _validateKeepCommand(const ASTNode *node);
    static bool _validateReplaceCommand(const ASTNode *node);
    static bool _validateEncloseCommand(const ASTNode *node);
    static bool _validateRedirectCommand(const ASTNode *node);
    static bool _validateSetCommand(const ASTNode *node);
    static bool _validateVacationCommand(const ASTNode *node);
    static bool _validateBareCommand(const ASTNode *node);
    static bool _validateSingleArgumentCommand(const ASTNode *node);
    static bool _validateSingleStringArgumentCommand(const ASTNode *node);
    static bool _validateBreakCommand(const ASTNode *node);
    static bool _validateForeverypartCommand(const ASTNode *node);
    static bool _validateExtracttextCommand(const ASTNode *node);
    static bool _validateExpireCommand(const ASTNode *node);
    static bool _validateNotifyCommand(const ASTNode *node);
    static bool _validateConvertCommand(const ASTNode *node);

    std::map<std::string, bool (*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
