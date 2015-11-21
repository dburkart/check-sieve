#ifndef __ASTBRANCH_HH__
#define __ASTBRANCH_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTBranch : public ASTNode {
public:
    ASTBranch() : ASTNode() {}    
    ASTBranch(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTBranch"; }
    
private:
};

} // namespace sieve

#endif
