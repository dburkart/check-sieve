#include "ASTBlock.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBlock::ASTBlock( yy::location location ) 
    : ASTNode( location ) 
{ 
}


void ASTBlock::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve