#include "ASTBoolean.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBoolean::ASTBoolean( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Boolean constant";
}

void ASTBoolean::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}