#ifndef __ASTTAG_HH__
#define __ASTTAG_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTTag : public ASTNode {
public:
    ASTTag() : ASTNode() {}    
    ASTTag(yy::location location);
    ASTTag(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return this->_name; }
    
private:
    std::string _name;
};

} // namespace sieve

#endif
