#include "ASTNumeric.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTNumeric::ASTNumeric( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Numeric constant";
}

void ASTNumeric::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}