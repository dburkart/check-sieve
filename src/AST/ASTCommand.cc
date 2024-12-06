#include <utility>

#include "ASTCommand.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTCommand::ASTCommand( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTCommand::ASTCommand( std::string name)
    : _name(std::move( name ))
{
}
ASTCommand::ASTCommand( yy::location location, std::string name)
    : ASTNode( location )
    , _name(std::move( name ))
{
}

void ASTCommand::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve