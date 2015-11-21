#include "ASTTag.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTTag::ASTTag( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTTag::ASTTag( std::string name)
    : _name( name )
{
}
ASTTag::ASTTag( yy::location location, std::string name)
    : ASTNode( location )
    , _name( name )
{
}

void ASTTag::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve