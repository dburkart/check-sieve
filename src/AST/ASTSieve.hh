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
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }
    

private:
    std::string _description;
};

} // namespace sieve

#endif
