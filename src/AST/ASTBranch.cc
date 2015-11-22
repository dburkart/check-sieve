#include "ASTBranch.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBranch::ASTBranch( yy::location location ) 
    : ASTNode( location ) 
{ 
}


void ASTBranch::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve