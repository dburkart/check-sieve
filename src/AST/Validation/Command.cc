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

    if (command->find(ASTTag(":flags")) != command->children().end()) {
        minArguments += 2;
    }

    if (command->find(ASTTag(":copy")) != command->children().end()) {
        minArguments++;
    }

    if (size < minArguments) {
        return false;
    }

    return true;
}

bool validateKeepCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 0;

    if (command->find(ASTTag(":flags")) != command->children().end()) {
        numArguments += 2;
    }

    if (size != numArguments) {
        return false;
    }

    return true;
}

bool validateReplaceCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 1;

    if (command->find(ASTTag(":mime")) != command->children().end()) {
        numArguments += 1;
    }

    if (command->find(ASTTag(":subject")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":from")) != command->children().end()) {
        numArguments += 2;
    }

    if (size != numArguments) {
        return false;
    }

    return true;
}

bool validateEncloseCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 1;

    if (command->find(ASTTag(":subject")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":headers")) != command->children().end()) {
        numArguments += 2;
    }

    if (size != numArguments) {
        return false;
    }

    return true;
}

bool validateSingleWordCommand(const ASTCommand *command) {
    size_t size = command->children().size();

    if (size == 0)
        return true;
    else
        return false;
}

Command::Command() {
    _usage_map["addflag"] = "addflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["enclose"] = "enclose <:subject string> <:headers string-list> string";
    _usage_map["fileinto"] = "fileinto [:flags <list-of-flags: string-list>][:copy] <folder: string>";
    _usage_map["include"] = "include [:global / :personal] [\":once\"] [\":optional\"] <value: string>";
    _usage_map["keep"] = "keep [:flags <list-of-flags: string-list>]";
    _usage_map["removeflag"] = "removeflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["replace"] = "replace [:mime] [:subject string] [:from string] <replacement: string>";
    _usage_map["setflag"] = "setflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["stop"] = "stop";

    _validation_fn_map["addflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["enclose"] = &validateEncloseCommand;
    _validation_fn_map["fileinto"] = &validateFileintoCommand;
    _validation_fn_map["include"] = &validateIncludeCommand;
    _validation_fn_map["keep"] = &validateKeepCommand;
    _validation_fn_map["removeflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["replace"] = &validateReplaceCommand;
    _validation_fn_map["setflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["stop"] = &validateSingleWordCommand;
}

bool Command::validate(const ASTCommand *command) {
    if (!_validation_fn_map[command->value()])
        return true;
    
    return _validation_fn_map[command->value()](command);
}

std::string Command::usage(const ASTCommand *command) {
    return "Usage: " + _usage_map[command->value()];
}

}