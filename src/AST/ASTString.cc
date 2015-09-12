#include "ASTString.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTString::ASTString( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "String constant";
}

void ASTString::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}