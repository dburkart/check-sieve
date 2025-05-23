#include <regex>
#include <unordered_set>

#include "ASTBlock.hh"
#include "ASTCommand.hh"
#include "ASTNumeric.hh"
#include "ASTString.hh"
#include "ASTStringList.hh"
#include "ASTTag.hh"
#include "checksieve.h"
#include "Command.hh"
#include "ASTVerificationVisitor.hh"

namespace sieve
{

Command::Command() {
    _usage_map["addflag"] = "addflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["addheader"] = "addheader [:last] <field-name: string> <value: string>";
    _usage_map["break"] = "break [:name string]";
    _usage_map["convert"]               = "convert  <quoted-from-media-type: string>                                      \n"
                                          "         <quoted-to-media-type: string>                                        \n"
                                          "         <transcoding-params: string-list>                                     \n";
    _usage_map["deleteheader"] = "deleteheader [:index <fieldno: number> [:last]]\n\t[COMPARATOR] [MATCH-TYPE]\n\t<field-name: string>\n\t[<value-patterns: string-list>]";
    _usage_map["discard"] = "discard";
    _usage_map["enclose"] = "enclose <:subject string> <:headers string-list> string";
    _usage_map["ereject"] = "ereject <reason: string>";
    _usage_map["error"] = "error <message: string>";
    _usage_map["extracttext"] = "extracttext [MODIFIER] [:first number] <varname: string>";
    _usage_map["expire"] = "expire <unit: day / minute / second> <value: string>";
    _usage_map["fileinto"] = "fileinto [:flags <list-of-flags: string-list>][:copy] <folder: string>";
    _usage_map["foreverypart"] = "foreverypart [:name string] block";
    _usage_map["global"] = "global <value: string-list>";
    _usage_map["include"] = "include [:global / :personal] [:once] [:optional] <value: string>";
    _usage_map["keep"] = "keep [:flags <list-of-flags: string-list>]";
    _usage_map["notify"] = "notify [:from string] [:importance <1 / 2 / 3>]\n\t[:options string-list] [:message string] [:fcc string] <method: string>";
    _usage_map["redirect"] = "redirect [:copy / :list] <address: string>";
    _usage_map["reject"] = "reject <reason: string>";
    _usage_map["removeflag"] = "removeflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["replace"] = "replace [:mime] [:subject string] [:from string] <replacement: string>";
    _usage_map["return"] = "return";
    _usage_map["set"] = "set [:modifier] <name: string> <value: string>";
    _usage_map["setflag"] = "setflag [<variablename: string>] <list-of-flags: string-list>";
    _usage_map["stop"] = "stop";
    _usage_map["unexpire"] = "unexpire";
    _usage_map["vacation"] = "vacation [:days number] [:subject string] [:from string]\n\t[:addresses string-list] [:mime] [:handle string]\n\t[:fcc string] <reason: string>";
    _usage_map["pipe"] = "pipe [:try] <program-name: string>\n\t[<arguments: string-list>]";
    _usage_map["filter"] = "filter <program-name: string> [<arguments: string-list>]";
    _usage_map["execute"] = "execute [:input <input-data: string> / :pipe]\n\t[:output <varname: string>]\n\t<program-name: string> [<arguments: string-list>]";
    _usage_map["processcalendar"] = "processcalendar [:allowpublic ]\n"
                                    "                [:addresses <string-list>]\n"
                                    "                [:updatesonly / :calendarid <string>]\n"
                                    "                [:deletecancelled]\n"
                                    "                [:organizers <ext-list-name: string>]\n"
                                    "                [:outcome <variable-name: string>]\n"
                                    "                [:reason <variable-name: string>]";

    _validation_fn_map["addflag"] = &Command::_validateIMAP4FlagsAction;
    _validation_fn_map["addheader"] = &Command::_validateAddHeadersCommand;
    _validation_fn_map["break"] = &Command::_validateBreakCommand;
    _validation_fn_map["deleteheader"] = &Command::_validateDeleteHeadersCommand;
    _validation_fn_map["discard"] = &Command::_validateBareCommand;
    _validation_fn_map["enclose"] = &Command::_validateEncloseCommand;
    _validation_fn_map["ereject"] = &Command::_validateSingleStringArgumentCommand;
    _validation_fn_map["error"] = &Command::_validateSingleStringArgumentCommand;
    _validation_fn_map["extracttext"] = &Command::_validateExtracttextCommand;
    _validation_fn_map["expire"] = &Command::_validateExpireCommand;
    _validation_fn_map["fileinto"] = &Command::_validateFileintoCommand;
    _validation_fn_map["foreverypart"] = &Command::_validateForeverypartCommand;
    _validation_fn_map["global"] = &Command::_validateSingleArgumentCommand;
    _validation_fn_map["include"] = &Command::_validateIncludeCommand;
    _validation_fn_map["keep"] = &Command::_validateKeepCommand;
    _validation_fn_map["notify"] = &Command::_validateNotifyCommand;
    _validation_fn_map["redirect"] = &Command::_validateRedirectCommand;
    _validation_fn_map["reject"] = &Command::_validateSingleArgumentCommand;
    _validation_fn_map["removeflag"] = &Command::_validateIMAP4FlagsAction;
    _validation_fn_map["replace"] = &Command::_validateReplaceCommand;
    _validation_fn_map["return"] = &Command::_validateBareCommand;
    _validation_fn_map["set"] = &Command::_validateSetCommand;
    _validation_fn_map["setflag"] = &Command::_validateIMAP4FlagsAction;
    _validation_fn_map["stop"] = &Command::_validateBareCommand;
    _validation_fn_map["unexpire"] = &Command::_validateBareCommand;
    _validation_fn_map["vacation"] = &Command::_validateVacationCommand;
    _validation_fn_map["convert"] = &Command::_validateConvertCommand;
    _validation_fn_map["pipe"] = &Command::_validatePipeCommand;
    _validation_fn_map["filter"] = &Command::_validateFilterCommand;
    _validation_fn_map["execute"] = &Command::_validateExecuteCommand;
    _validation_fn_map["processcalendar"] = &Command::_validateProcesscalendarCommand;
}

ValidationResult Command::validate(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);

    if (!_validation_fn_map[command->value()]) {
        DEBUGLOG(command->value() + " command is missing validation.")
        return ValidationResult(true);
    }

    return (this->_validation_fn_map[command->value()])(command);
}

std::string Command::usage(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    return "Usage: " + _usage_map[command->value()];
}

//-- Private members
ValidationResult Command::_validateAddHeadersCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = command->children().size();
    
    if (size != 2 && size != 3)
        return ValidationResult(false);
    
    int i = 0;
    for (const auto it : children) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(it);
        const ASTString *stringChild = dynamic_cast<ASTString *>(it);
    
        // If we have 3 children, the first child must be a :last tag.
        if (size == 3 && i == 0) {
            if (!tagChild || tagChild->value() != ":last")
                return ValidationResult(false);
        }
        // Every other child must be a string
        else if (!stringChild) {
            return ValidationResult(false);
        }
    
        i++;
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateDeleteHeadersCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    std::vector<ASTNode *> children = command->children();
    const size_t size = command->children().size();
    size_t minSize = 1;
    
    // :index
    auto indexTag = command->find(ASTTag(":index"));
    if (indexTag != command->children().end()) {

        // Must be the first child
        if (indexTag != command->children().begin())
            return ValidationResult(false);
    
        minSize += 2;
        auto it = indexTag + 1;
    
        // Ensure that the next argument is a numeric
        //const T* child = dynamic_cast<T*>(*it);
        const auto numeric = dynamic_cast<ASTNumeric*>(*it);
        if (numeric == nullptr)
            return ValidationResult(false);
    
        ++indexTag;
        it = indexTag + 1;
    
        // Allow a :last tag, but only if it's the next child
        const auto lastTag = command->find(ASTTag(":last"));
        if (lastTag != command->children().end() && lastTag != it)
            return ValidationResult(false);
    
        if (lastTag != command->children().end() && lastTag == it) {
            ++indexTag;
            minSize += 1;
        }
    } else {
        // TODO: Ensure that :last hasn't been specified since their's no :index
        if (indexTag != command->children().end())
            return ValidationResult(false);
    }
    
    // TODO: Validate comparators
    
    if (size < minSize)
        return ValidationResult(false);
    
    // TODO: Validate string-list
    
    return ValidationResult(true);
}

// Validation logic
// TODO: Think about if this is the right way to do this
ValidationResult Command::_validateIncludeCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    if (size < 5 && size > 0)
        return ValidationResult(true);

    return ValidationResult(false);
}

ValidationResult Command::_validateIMAP4FlagsAction(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size > 0 && size < 3)
        return ValidationResult(true);

    return ValidationResult(false);
}

ValidationResult Command::_validateFileintoCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    if (size < 1)
        return ValidationResult(false);
    
    size_t minArguments = 1;
    
    if (command->find(ASTTag(":flags")) != command->children().end()) {
        minArguments += 2;
    }

    if (command->find(ASTTag(":copy")) != command->children().end()) {
        minArguments++;
    }

    if (size < minArguments) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateKeepCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 0;
    
    if (command->find(ASTTag(":flags")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (size != numArguments) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateReplaceCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 1;
    
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
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateEncloseCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 1;
    
    if (command->find(ASTTag(":subject")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (command->find(ASTTag(":headers")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (size != numArguments) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateExpireCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand *>(node);
    const auto children = command->children();
    const size_t size = children.size();

    if (size != 2) {
        return ValidationResult(false);
    }

    const ASTString *unitChild = dynamic_cast<ASTString *>(command->children()[0]);
    const ASTString *valueChild = dynamic_cast<ASTString *>(command->children()[1]);
    if (unitChild == nullptr || valueChild == nullptr) {
        return ValidationResult(false);
    }

    const std::unordered_set<std::string_view> units = {"day", "minute", "second"};
    if (units.find(unitChild->value()) == units.end()) {
        return ValidationResult(false);
    }

    if (valueChild->value().find_first_not_of("0123456789") != std::string::npos) {
        return ValidationResult(false);
    }

    return ValidationResult(true);
}

ValidationResult Command::_validateRedirectCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 1;

    for (auto & it : command->children()) {
        const auto *arg = dynamic_cast<const ASTTag*>(it);

        if (arg == nullptr)
            continue;

        if (arg->value() == ":copy" || arg->value() == ":list" || arg->value() == ":bytrace")
            numArguments += 1;

        if (arg->value() == ":notify" || arg->value() == ":ret" || arg->value() == ":bytimerelative" ||
            arg->value() == ":bytimeabsolute" || arg->value() == ":bymode")
            numArguments += 2;
    }

    if (size != numArguments) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateSetCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 2;
    size_t stringArguments = 0;
    
    for (const auto it : children) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(it);
        const ASTString *stringChild = dynamic_cast<ASTString *>(it);
    
        if (tagChild && (
            tagChild->value() == ":lower" ||
            tagChild->value() == ":upper" ||
            tagChild->value() == ":lowerfirst" ||
            tagChild->value() == ":upperfirst" ||
            tagChild->value() == ":quotewildcard" ||
            tagChild->value() == ":length" ||
            tagChild->value() == ":quoteregex" ||
            tagChild->value() == ":encodeurl" ||
            tagChild->value() == ":eval"
           ))
            numArguments += 1;
        else if (tagChild)
            return ValidationResult(false);
    
        if (stringChild)
            stringArguments++;
    
        if (stringArguments > 2)
            return ValidationResult(false);
    }

    if (size != numArguments || stringArguments < 2) {
        return ValidationResult(false);
    }
    
    // TODO: We blindly allow namespaces in variable names even though they are
    //       disallowed unless the inclusion of an extension enabling that
    //       namespace is required.
    const auto variableName = std::string{ dynamic_cast<const ASTString*>(children[size-2])->value() };
    const std::regex identifierOrDigit("^([a-zA-Z0-9_\\.]+|[0-9])$");
    
    if (!std::regex_match(variableName, identifierOrDigit)) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateVacationCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const auto children = command->children();
    const size_t size = children.size();
    
    size_t numArguments = 1;
    
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

    if (command->find(ASTTag(":fcc")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (size != numArguments) {
        return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateBareCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size == 0)
        return ValidationResult(true);

    return ValidationResult(false);
}

ValidationResult Command::_validateSingleArgumentCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size == 1)
        return ValidationResult(true);

    return ValidationResult(false);
}

ValidationResult Command::_validateSingleStringArgumentCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size != 1)
        return ValidationResult(false);
    
    const auto *argument = dynamic_cast<const ASTString*>(command->children()[0]);
    if (argument == nullptr)
        return ValidationResult(false);
    
    return ValidationResult(true);
}

ValidationResult Command::_validateBreakCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size == 0)
        return ValidationResult(true);
    
    if (size != 2)
        return ValidationResult(false);
    
    const ASTTag *tagChild = dynamic_cast<ASTTag *>(command->children()[0]);
    const ASTString *stringChild = dynamic_cast<ASTString *>(command->children()[1]);
    
    if (tagChild == nullptr || stringChild == nullptr)
        return ValidationResult(false);
    
    return ValidationResult(true);
}

ValidationResult Command::_validateForeverypartCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size != 1 && size != 3)
        return ValidationResult(false);
    
    if (!nodeIsType<ASTBlock>(command->children()[size-1]))
        return ValidationResult(false);
    
    if (size == 3) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(command->children()[0]);
        const ASTString *stringChild = dynamic_cast<ASTString *>(command->children()[1]);
    
        if (tagChild == nullptr || stringChild == nullptr)
            return ValidationResult(false);
    }
    
    return ValidationResult(true);
}

ValidationResult Command::_validateExtracttextCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    if (!nodeIsType<ASTString>(command->children()[size-1]))
        return ValidationResult(false);
    
    return ValidationResult(true);
}

ValidationResult Command::_validateNotifyCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    size_t numArguments = 1;
    
    if (!nodeIsType<ASTString>(command->children()[size-1]))
        return ValidationResult(false);
    
    if (command->find(ASTTag(":from")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (command->find(ASTTag(":importance")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (command->find(ASTTag(":options")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (command->find(ASTTag(":message")) != command->children().end()) {
        numArguments += 2;
    }

    if (command->find(ASTTag(":fcc")) != command->children().end()) {
        numArguments += 2;
    }
    
    if (size != numArguments)
        return ValidationResult(false);
    
    return ValidationResult(true);
}

ValidationResult Command::_validateConvertCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();
    
    if (size != 3)
        return ValidationResult(false);
    
    // TODO: We need to verify that the strings are quoted
    const ASTString *fromMediaType = dynamic_cast<ASTString *>(command->children()[0]);
    const ASTString *toMediaType = dynamic_cast<ASTString *>(command->children()[1]);
    const ASTStringList *transcodingParams = dynamic_cast<ASTStringList *>(command->children()[2]);
    
    if (fromMediaType == nullptr || toMediaType == nullptr || transcodingParams == nullptr)
        return ValidationResult(false);
    
    return ValidationResult(true);
}

ValidationResult Command::_validatePipeCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    if (size == 0)
        return ValidationResult(false);

    // If size == 1, verify that the argument is a string
    if (size == 1) {
        if (nodeIsType<ASTString>(command->children()[0]))
            return ValidationResult(true);
        else
            return ValidationResult(false);
    }

    bool seenProgram = false;
    for (size_t i = 0; i < size; i++) {
        if (nodeIsType<ASTString>(command->children()[i])) {
            seenProgram = true;
            continue;
        }

        if (nodeIsType<ASTTag>(command->children()[i])) {
            if (seenProgram) {
                return {false, "Note: tag arguments not allowed after program name", true, int(i)};
            }

            const auto *tag = dynamic_cast<const ASTTag*>(command->children()[i]);
            if (tag->value() != ":try" && tag->value() != ":copy") {
                return {false, "Note: only :try and :copy allowed as arguments to pipe", true, int(i)};
            }
        }

        if (seenProgram) {
            if (!nodeIsType<ASTStringList>(command->children()[i])) {
                return { false, "Expected program arguments", true, int(i)};
            }
        }
    }

    return ValidationResult(true);
}

ValidationResult Command::_validateFilterCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    if (size == 0)
        return ValidationResult(false);

    if (!nodeIsType<ASTString>(command->children()[0])) {
        return {false, "First argument to filter command must be a program name", true};
    }

    if (size > 1) {
        if (!nodeIsType<ASTStringList>(command->children()[1])) {
            return ValidationResult(false);
        }
    }

    return ValidationResult(true);
}

ValidationResult Command::_validateExecuteCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    size_t minSize = 1;

    if (size == 0)
        return ValidationResult(false);

    if (command->find(ASTTag(":input")) != command->children().end())
        minSize += 2;

    if (command->find(ASTTag(":output")) != command->children().end())
        minSize += 2;

    if (command->find(ASTTag(":pipe")) != command->children().end())
        minSize += 1;


    if (nodeIsType<ASTStringList>(command->children()[size-1]))
        minSize += 1;

    if (size < minSize)
        return ValidationResult(false);

    return ValidationResult(true);
}

ValidationResult Command::_validateProcesscalendarCommand(const ASTNode *node) {
    const auto *command = dynamic_cast<const ASTCommand*>(node);
    const size_t size = command->children().size();

    size_t desiredSize = 0;
    int i = 0;

    for (const auto it : command->children()) {
        const ASTTag *tagChild = dynamic_cast<ASTTag *>(it);
        i++;

        if (tagChild && (
                        tagChild->value() == ":allowpublic" ||
                        tagChild->value() == ":updatesonly" ||
                        tagChild->value() == ":deletecancelled"
                )) {
            desiredSize += 1;
        } else if (tagChild && (
                        tagChild->value() == ":addresses" ||
                        tagChild->value() == ":calendarid" ||
                        tagChild->value() == ":organizers" ||
                        tagChild->value() == ":outcome" ||
                        tagChild->value() == ":reason"
                )) {
            desiredSize += 2;
        } else if (tagChild) {
            auto reason = "Unexpected tag found: " + tagChild->value();
            return ValidationResult(false, reason, false, i);
        }
    }

    if (command->find(ASTTag(":updatesonly")) != command->children().end()) {
        if (command->find(ASTTag(":calendarid")) != command->children().end())
            return ValidationResult(false, ":updatesonly and :calendarid are mutually exclusive", true);
    }

    if (size != desiredSize)
        return ValidationResult(false);

    return ValidationResult(true);
}

}
