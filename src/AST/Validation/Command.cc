#include "ASTBlock.hh"
#include "ASTNumeric.hh"
#include "ASTString.hh"
#include "ASTStringList.hh"
#include "ASTTag.hh"
#include "checksieve.h"
#include "Command.hh"

namespace sieve
{

bool validateAddHeadersCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    std::vector<sieve::ASTNode *> children = command->children();
    size_t size = command->children().size();

    if (size != 2 && size != 3)
        return false;

    int i = 0;
    for (std::vector<ASTNode *>::const_iterator it = children.begin(); it != children.end(); ++it) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(*it);
        const ASTString *stringChild = dynamic_cast<ASTString *>(*it);

        // If we have 3 children, the first child must be a :last tag.
        if (size == 3 && i == 0) {
            if (!tagChild || tagChild->value() != ":last")
                return false;
        }
        // Every other child must be a string
        else if (!stringChild) {
            return false;
        }

        i++;
    }

    return true;
}

bool validateDeleteHeadersCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    std::vector<sieve::ASTNode *> children = command->children();
    std::vector<ASTNode *>::const_iterator it;
    size_t size = command->children().size();
    size_t minSize = 1;

    ASTNumeric* numeric;
    ASTTag* tag;

    // :index
    std::vector<ASTNode *>::const_iterator indexTag = command->find(ASTTag(":index"));
    if (indexTag != command->children().end()) {
        // Must be the first child
        if (indexTag != command->children().begin())
            return false;

        minSize += 2;
        it = indexTag + 1;

        // Ensure that the next argument is a numeric
        //const T* child = dynamic_cast<T*>(*it);
        numeric = dynamic_cast<ASTNumeric*>(*it);
        if (numeric == NULL)
            return false;

        indexTag++;
        it = indexTag + 1;

        // Allow a :last tag, but only if it's the next child
        std::vector<ASTNode *>::const_iterator lastTag = command->find(ASTTag(":last"));
        if (lastTag != command->children().end() && lastTag != it)
            return false;

        if (lastTag != command->children().end() && lastTag == it) {
            indexTag++;
            minSize += 1;
        }
    } else {
        // Ensure that :last hasn't been specified since their's no :index
        std::vector<ASTNode *>::const_iterator lastTag = command->find(ASTTag(":last"));

        if (indexTag != command->children().end())
            return false;
    }

    // TODO: Validate comparators
    // it = indexTag + 1;
    // tag = dynamic_cast<ASTTag*>(*it);
    // if (tag)
    //     minSize += 1;

    if (size < minSize)
        return false;

    // TODO: Validate string-list

    return true;
}


// Validation logic
// TODO: Think about if this is the right way to do this
bool validateIncludeCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size < 5 && size > 0)
        return true;
    else
        return false;
}

bool validateIMAP4FlagsAction(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size > 0 && size < 3)
        return true;
    else
        return false;
}

bool validateFileintoCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateKeepCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateReplaceCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateEncloseCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateRedirectCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateSetCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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
            tagChild->value() == ":length" ||
            tagChild->value() == ":quoteregex"
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

bool validateVacationCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
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

bool validateBareCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size == 0)
        return true;
    else
        return false;
}

bool validateSingleArgumentCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size == 1)
        return true;
    else
        return false;
}

bool validateBreakCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size == 0)
        return true;

    if (size != 2)
        return false;

    const ASTTag *tagChild = dynamic_cast<ASTTag *>(command->children()[0]);
    const ASTString *stringChild = dynamic_cast<ASTString *>(command->children()[1]);

    if (tagChild == NULL || stringChild == NULL)
        return false;

    return true;
}

bool validateForeverypartCommand(const ASTNode *node) {
    const ASTCommand *command = dynamic_cast<const ASTCommand*>(node);
    size_t size = command->children().size();

    if (size != 1 && size != 3)
        return false;

    const ASTBlock *blockChild = dynamic_cast<ASTBlock *>(command->children()[size-1]);
    if (blockChild == NULL)
        return false;

    if (size == 3) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(command->children()[0]);
        const ASTString *stringChild = dynamic_cast<ASTString *>(command->children()[1]);

        if (tagChild == NULL || stringChild == NULL)
            return false;
    }

    return true;
}

Command::Command() {
    _usage_map["addflag"] = "addflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["addheader"] = "addheader [:last] <field-name: string> <value: string>";
    _usage_map["break"] = "break [:name string]";
    _usage_map["deleteheader"] = "deleteheader [:index <fieldno: number> [:last]]\n\t[COMPARATOR] [MATCH-TYPE]\n\t<field-name: string>\n\t[<value-patterns: string-list>]";
    _usage_map["discard"] = "discard";
    _usage_map["enclose"] = "enclose <:subject string> <:headers string-list> string";
    _usage_map["ereject"] = "ereject <reason: string>";
    _usage_map["fileinto"] = "fileinto [:flags <list-of-flags: string-list>][:copy] <folder: string>";
    _usage_map["foreverypart"] = "foreverypart [:name string] block";
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
    _validation_fn_map["addheader"] = &validateAddHeadersCommand;
    _validation_fn_map["break"] = &validateBreakCommand;
    _validation_fn_map["deleteheader"] = &validateDeleteHeadersCommand;
    _validation_fn_map["discard"] = &validateBareCommand;
    _validation_fn_map["enclose"] = &validateEncloseCommand;
    _validation_fn_map["ereject"] = &validateSingleArgumentCommand;
    _validation_fn_map["fileinto"] = &validateFileintoCommand;
    _validation_fn_map["foreverypart"] = &validateForeverypartCommand;
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
    if (!_validation_fn_map[command->value()]) {
        DEBUGLOG(command->value(), "Command is missing validation.")
        return true;
    }

    return _validation_fn_map[command->value()](command);
}

std::string Command::usage(const ASTCommand *command) {
    return "Usage: " + _usage_map[command->value()];
}

}
