#include "ASTNoOp.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTNoOp::ASTNoOp( yy::location location ) 
    : ASTNode( location ) 
{ 
}


void ASTNoOp::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve