#include "ASTCondition.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTCondition::ASTCondition( yy::location location ) 
    : ASTNode( location ) 
{ 
}


void ASTCondition::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve