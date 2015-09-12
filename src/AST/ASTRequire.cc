#include "ASTRequire.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTRequire::ASTRequire( yy::location location ) 
    : ASTCommand( location ) {
    this->_description = "Require statement";
}


void ASTRequire::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve