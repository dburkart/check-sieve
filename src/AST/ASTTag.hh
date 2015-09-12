#ifndef __ASTTAG_HH__
#define __ASTTAG_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTTag : public ASTNode {
public:
    
    ASTTag(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }

private:
    std::string _description;
};

} // namespace sieve

#endif
