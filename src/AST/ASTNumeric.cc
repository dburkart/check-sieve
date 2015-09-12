#include "ASTNumeric.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTNumeric::ASTNumeric( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Numeric constant";
}

ASTNumeric::ASTNumeric( yy::location location, int number)
    : ASTNode( location )
    , _number( number ) {
    this->_description = "Numeric constant";
}

void ASTNumeric::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve