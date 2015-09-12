#ifndef __ASTSTRING_HH__
#define __ASTSTRING_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTString : public ASTNode {
public:
    
    ASTString(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }

private:
    std::string _description;
};

} // namespace sieve

#endif
