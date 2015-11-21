#ifndef __ASTBLOCK_HH__
#define __ASTBLOCK_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTBlock : public ASTNode {
public:
    ASTBlock() : ASTNode() {}    
    ASTBlock(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTBlock"; }
    
private:
};

} // namespace sieve

#endif
