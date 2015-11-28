#include "ASTString.hh"
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

bool validateRedirectCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 1;

    if (command->find(ASTTag(":copy")) != command->children().end()) {
        numArguments += 1;
    }

    if (size != numArguments) {
        return false;
    }

    return true;
}

bool validateSetCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 2;
    int stringArguments = 0;

    for (std::vector<ASTNode *>::const_iterator it = children.begin(); it != children.end(); ++it) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(*it);
        const ASTString *stringChild = dynamic_cast<ASTString *>(*it);

        if (tagChild && (
            tagChild->value() == ":lower" ||
            tagChild->value() == ":upper" ||
            tagChild->value() == ":lowerfirst" ||
            tagChild->value() == ":upperfirst" ||
            tagChild->value() == ":quotewildcard" ||
            tagChild->value() == ":length"
           ))
            numArguments += 1;
        else if (tagChild)
            return false;

        if (stringChild)
            stringArguments++;

        if (stringArguments > 2)
            return false;
    }

    if (size != numArguments || stringArguments < 2) {
        return false;
    }

    return true;
}

bool validateVacationCommand(const ASTCommand *command) {
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = children.size();

    int numArguments = 1;

    if (command->find(ASTTag(":days")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":subject")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":from")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":addresses")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":mime")) != command->children().end()) {
        numArguments += 1;
    }

    if (command->find(ASTTag(":handle")) != command->children().end()) {
        numArguments += 2;
    }

    if (size != numArguments) {
        return false;
    }

    return true;
}

bool validateBareCommand(const ASTCommand *command) {
    size_t size = command->children().size();

    if (size == 0)
        return true;
    else
        return false;
}

bool validateSingleArgumentCommand(const ASTCommand *command) {
    size_t size = command->children().size();

    if (size == 1)
        return true;
    else
        return false;
}

Command::Command() {
    _usage_map["addflag"] = "addflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["discard"] = "discard";
    _usage_map["enclose"] = "enclose <:subject string> <:headers string-list> string";
    _usage_map["ereject"] = "ereject <reason: string>";
    _usage_map["fileinto"] = "fileinto [:flags <list-of-flags: string-list>][:copy] <folder: string>";
    _usage_map["global"] = "global <value: string-list>";
    _usage_map["include"] = "include [:global / :personal] [:once] [:optional] <value: string>";
    _usage_map["keep"] = "keep [:flags <list-of-flags: string-list>]";
    _usage_map["redirect"] = "redirect [:copy] <address: string>";
    _usage_map["reject"] = "reject <reason: string>";
    _usage_map["removeflag"] = "removeflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["replace"] = "replace [:mime] [:subject string] [:from string] <replacement: string>";
    _usage_map["return"] = "return";
    _usage_map["set"] = "set [:modifier] <name: string> <value: string>";
    _usage_map["setflag"] = "setflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["stop"] = "stop";
    _usage_map["vacation"] = "vacation [:days number] [:subject string] [:from string]\n\t[:addresses string-list] [:mime] [:handle string] <reason: string>";

    _validation_fn_map["addflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["discard"] = &validateBareCommand;
    _validation_fn_map["enclose"] = &validateEncloseCommand;
    _validation_fn_map["ereject"] = &validateSingleArgumentCommand;
    _validation_fn_map["fileinto"] = &validateFileintoCommand;
    _validation_fn_map["global"] = &validateSingleArgumentCommand;
    _validation_fn_map["include"] = &validateIncludeCommand;
    _validation_fn_map["keep"] = &validateKeepCommand;
    _validation_fn_map["redirect"] = &validateRedirectCommand;
    _validation_fn_map["reject"] = &validateSingleArgumentCommand;
    _validation_fn_map["removeflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["replace"] = &validateReplaceCommand;
    _validation_fn_map["return"] = &validateBareCommand;
    _validation_fn_map["set"] = &validateSetCommand;
    _validation_fn_map["setflag"] = &validateIMAP4FlagsAction;
    _validation_fn_map["stop"] = &validateBareCommand;
    _validation_fn_map["vacation"] = &validateVacationCommand;
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