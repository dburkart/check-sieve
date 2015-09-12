#include "ASTCommand.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTCommand::ASTCommand( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Command";
}

ASTCommand::ASTCommand( yy::location location, std::string name)
    : ASTNode( location )
    , _name( name ) {
    this->_description = "Command";
}

void ASTCommand::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve