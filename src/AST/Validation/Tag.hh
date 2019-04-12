#ifndef __TAG_H__
#define __TAG_H__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTTag.hh"

namespace sieve 
{

class Tag {
public:
    Tag();
    ~Tag() {}

    bool validate(const ASTTag *tag);
    std::string usage(const ASTTag *tag);

private:
    std::map<std::string, ValidationFunctionPtr> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif
