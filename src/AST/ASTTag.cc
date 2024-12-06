#include <utility>

#include "ASTTag.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTTag::ASTTag( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTTag::ASTTag( std::string name)
    : _name(std::move( name ))
{
}
ASTTag::ASTTag( yy::location location, std::string name)
    : ASTNode( location )
    , _name(std::move( name ))
{
}

void ASTTag::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve