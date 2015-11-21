#include "ASTBoolean.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBoolean::ASTBoolean( yy::location location ) 
    : ASTTest( location ) 
{ 
}

ASTBoolean::ASTBoolean( bool val)
    : _val( val )
{
}
ASTBoolean::ASTBoolean( yy::location location, bool val)
    : ASTTest( location )
    , _val( val )
{
}

void ASTBoolean::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve