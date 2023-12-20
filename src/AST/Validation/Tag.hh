#pragma once

#include <map>
#include <string>

#include "Validation.hh"

namespace sieve 
{

class Tag : public Validator{
public:
    Tag();
    ~Tag() = default;

    bool validate(const ASTNode *tag) override;
    std::string usage(const ASTNode *tag) override;

private:
    // Validation functions
    bool _validateSingleString(const ASTNode *node);
    bool _validateSingleNumeric(const ASTNode *node);
    bool _validateList(const ASTNode *node);
    
    std::map<std::string, bool (Tag::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
