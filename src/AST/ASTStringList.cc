#include "ASTStringList.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTStringList::ASTStringList( yy::location location ) 
    : ASTNode( location ) 
{ 
}


void ASTStringList::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve