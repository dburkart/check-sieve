#ifndef __COMMAND_HH__
#define __COMMAND_HH__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTCommand.hh"

namespace sieve 
{

class Command {
public:
    Command();
    ~Command() {}

    bool validate(const ASTCommand *command);
    std::string usage(const ASTCommand *command);

private:
    std::map<std::string, ValidationFunctionPtr> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif
