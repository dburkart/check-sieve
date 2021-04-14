#ifndef __TEST_H__
#define __TEST_H__

#include <map>
#include <string>

#include "Validation.hh"
#include "ASTTest.hh"

namespace sieve 
{

class Test : public Validator {
public:
    Test();
    ~Test() {}

    bool validate(const ASTNode *command);
    std::string usage(const ASTNode *command);

private:
    std::map<std::string, ValidationFunctionPtr> _validation_fn_map;
    std::map<std::string, std::string> _usage_map;
};

}

#endif