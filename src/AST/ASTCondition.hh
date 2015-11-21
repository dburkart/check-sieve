#ifndef __ASTCONDITION_HH__
#define __ASTCONDITION_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTCondition : public ASTNode {
public:
    ASTCondition() : ASTNode() {}    
    ASTCondition(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTCondition"; }
    
private:
};

} // namespace sieve

#endif
