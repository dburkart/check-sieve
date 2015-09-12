#include "ASTCondition.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTCondition::ASTCondition( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Control flow tests";
}


void ASTCondition::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve