#ifndef __ASTSIEVE_HH__
#define __ASTSIEVE_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTSieve : public ASTNode {
public:
    ASTSieve() : ASTNode() {}    
    ASTSieve(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTSieve"; }
    
private:
};

} // namespace sieve

#endif
