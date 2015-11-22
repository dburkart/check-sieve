#include "ASTTag.hh"
#include "Command.hh"

namespace sieve 
{

// Validation logic
// TODO: Think about if this is the right way to do this
bool validateIncludeCommand(const ASTCommand *command) {
    size_t size = command->children().size();
    
    if (size < 5 && size > 0)
        return true;
    else
        return false;
}

bool validateIMAP4FlagsAction(const ASTCommand *command) {
    size_t size = command->children().size();

    if (size > 0 && size < 3)
        return true;
    else
        return false;
}

bool validateFileintoCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    if (size < 1)
        return false;

    int minArguments = 1;

    if (command->find(ASTTag(":flags")) != NULL) {
        minArguments += 2;
    }

    if (command->find(ASTTag(":copy")) != NULL) {
        minArguments++;
    }

    if (children.size() < minArguments) {
        return false;
    }

    return true;
}

Command::Command() {
    _usage_map["include"] = "include [:global / :personal] [\":once\"] [\":optional\"] <value: string>";
    _usage_map["setflag"] = "setflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["addflag"] = "addflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["fileinto"] = "fileinto [:flags <list-of-flags: string-list>][:copy] <folder: string>";

    _validation_fn_map["include"] = &validateIncludeCommand;
    _validation_fn_map["setflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["addflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["fileinto"] = &validateFileintoCommand;
}

bool Command::validate(const ASTCommand *command) {
    if (!_validation_fn_map[command->value()])
        return true;
    
    return _validation_fn_map[command->value()](command);
}

std::string Command::usage(const ASTCommand *command) {
    return _usage_map[command->value()];
}

}