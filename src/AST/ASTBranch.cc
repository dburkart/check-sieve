#include "ASTBranch.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBranch::ASTBranch( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Control flow branch";
}

void ASTBranch::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

}