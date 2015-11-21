#ifndef __ASTNOOP_HH__
#define __ASTNOOP_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTNoOp : public ASTNode {
public:
    ASTNoOp() : ASTNode() {}    
    ASTNoOp(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTNoOp"; }
    
private:
};

} // namespace sieve

#endif
