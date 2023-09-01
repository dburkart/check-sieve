#ifndef __COMMAND_HH__
#define __COMMAND_HH__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTCommand.hh"

namespace sieve 
{

class Command : public Validator {
public:
    Command();
    ~Command() = default;

    bool validate(const ASTNode *command) override;
    std::string usage(const ASTNode *command) override;

private:
    // Validation functions
    bool _validateAddHeadersCommand(const ASTNode *node);
    bool _validateDeleteHeadersCommand(const ASTNode *node);
    bool _validateIncludeCommand(const ASTNode *node);
    bool _validateIMAP4FlagsAction(const ASTNode *node);
    bool _validateFileintoCommand(const ASTNode *node);
    bool _validateKeepCommand(const ASTNode *node);
    bool _validateReplaceCommand(const ASTNode *node);
    bool _validateEncloseCommand(const ASTNode *node);
    bool _validateRedirectCommand(const ASTNode *node);
    bool _validateSetCommand(const ASTNode *node);
    bool _validateVacationCommand(const ASTNode *node);
    bool _validateBareCommand(const ASTNode *node);
    bool _validateSingleArgumentCommand(const ASTNode *node);
    bool _validateSingleStringArgumentCommand(const ASTNode *node);
    bool _validateBreakCommand(const ASTNode *node);
    bool _validateForeverypartCommand(const ASTNode *node);
    bool _validateExtracttextCommand(const ASTNode *node);
    bool _validateExpireCommand(const ASTNode *node);
    bool _validateNotifyCommand(const ASTNode *node);
    bool _validateConvertCommand(const ASTNode *node);

    std::map<std::string, bool (Command::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif
