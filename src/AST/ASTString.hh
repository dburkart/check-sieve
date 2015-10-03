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
    
    std::string value() const { return this->_str; }
    
private:
    std::string _str;
};

} // namespace sieve

#endif
