#include "ASTString.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTString::ASTString( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTString::ASTString( yy::location location, std::string str)
    : ASTNode( location )
    , _str( str )
{
}

void ASTString::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve