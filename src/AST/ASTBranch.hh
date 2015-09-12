#ifndef __ASTBRANCH_HH__
#define __ASTBRANCH_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTBranch : public ASTNode {
public:
    ASTBranch() : ASTNode() {}    
    ASTBranch(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    
private:
};

} // namespace sieve

#endif
