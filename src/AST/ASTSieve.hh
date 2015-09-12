#ifndef __ASTSIEVE_HH__
#define __ASTSIEVE_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTSieve : public ASTNode {
public:
    ASTSieve() : ASTNode() {}    
    ASTSieve(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    
private:
};

} // namespace sieve

#endif
