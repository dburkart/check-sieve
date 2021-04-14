#ifndef __TAG_H__
#define __TAG_H__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTTag.hh"

namespace sieve 
{

class Tag : public Validator{
public:
    Tag();
    ~Tag() {}

    bool validate(const ASTNode *tag);
    std::string usage(const ASTNode *tag);

private:
    // Validation functions
    bool _validateSingleString(const ASTNode *node);
    bool _validateSingleNumeric(const ASTNode *node);
    
    std::map<std::string, bool (Tag::*)(const ASTNode *)> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif
