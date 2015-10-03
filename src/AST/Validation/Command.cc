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

Command::Command() {
    _usage_map["include"] = "include [:global / :personal] [\":once\"] [\":optional\"] <value: string>";

    _validation_fn_map["include"] = &validateIncludeCommand;
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