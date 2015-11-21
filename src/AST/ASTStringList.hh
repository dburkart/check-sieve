#ifndef __ASTSTRINGLIST_HH__
#define __ASTSTRINGLIST_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTStringList : public ASTNode {
public:
    ASTStringList() : ASTNode() {}    
    ASTStringList(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTStringList"; }
    
private:
};

} // namespace sieve

#endif
