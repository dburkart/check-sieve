#ifndef __ASTBOOLEAN_HH__
#define __ASTBOOLEAN_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTBoolean : public ASTNode {
public:
    ASTBoolean() : ASTNode() {}    
    ASTBoolean(yy::location location);
    ASTBoolean(yy::location location, bool val);
    
    void accept(ASTVisitor &visitor);
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }
    
    bool value() { return this->_val; }

private:
    std::string _description;
    bool _val;
};

} // namespace sieve

#endif
