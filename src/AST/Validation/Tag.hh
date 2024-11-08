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

    bool validate(const ASTNode *node) override;
    std::string usage(const ASTNode *node) override;

private:
    // Validation functions
    static bool _validateSingleString(const ASTNode *node);
    static bool _validateSingleNumeric(const ASTNode *node);
    static bool _validateList(const ASTNode *node);
    static bool _validateSpecialUse(const ASTNode *node);
    
    std::map<std::string, bool (*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}
