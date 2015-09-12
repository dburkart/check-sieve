#include "ASTCommand.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTCommand::ASTCommand( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Command";
}

void ASTCommand::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}