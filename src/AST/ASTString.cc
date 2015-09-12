#include "ASTString.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTString::ASTString( yy::location location ) 
    : ASTNode( location ) {
    this->_description = "String constant";
}

ASTString::ASTString( yy::location location, std::string str)
    : ASTNode( location )
    , _str( str ) {
    this->_description = "String constant";
}

void ASTString::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve