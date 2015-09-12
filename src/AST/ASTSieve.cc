#include "ASTSieve.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTSieve::ASTSieve( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Mail Sieve";
}


void ASTSieve::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve