#include <utility>

#include "ASTTestList.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTTestList::ASTTestList( yy::location location ) 
    : ASTCommand( location ) 
{ 
}

ASTTestList::ASTTestList( std::string name)
    : _name(std::move( name ))
{
}
ASTTestList::ASTTestList( yy::location location, std::string name)
    : ASTCommand( location )
    , _name(std::move( name ))
{
}

void ASTTestList::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve