#ifndef __ASTSTRING_HH__
#define __ASTSTRING_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTString : public ASTNode {
public:
    ASTString() : ASTNode() {}    
    ASTString(yy::location location);
    ASTString(yy::location location, std::string str);
    
    void accept(ASTVisitor &visitor);
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }
    
    std::string value() { return this->_str; }

private:
    std::string _description;
    std::string _str;
};

} // namespace sieve

#endif
