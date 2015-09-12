#include "ASTTag.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTTag::ASTTag( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Tag constant";
}

void ASTTag::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}