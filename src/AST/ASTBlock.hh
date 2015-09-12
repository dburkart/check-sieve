#ifndef __ASTBLOCK_HH__
#define __ASTBLOCK_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTBlock : public ASTNode {
public:
    ASTBlock() : ASTNode() {}    
    ASTBlock(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    
private:
};

} // namespace sieve

#endif
