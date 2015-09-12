#include "ASTNumeric.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTNumeric::ASTNumeric( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTNumeric::ASTNumeric( yy::location location, int number)
    : ASTNode( location )
    , _number( number )
{
}

void ASTNumeric::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve