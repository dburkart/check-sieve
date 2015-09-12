#include "ASTBoolean.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTBoolean::ASTBoolean( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "Boolean constant";
}

ASTBoolean::ASTBoolean( yy::location location, bool val)
    : ASTNode( location )
    , _val( val ) {
    this->_description = "Boolean constant";
}

void ASTBoolean::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve