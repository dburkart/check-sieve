#include <utility>

#include "ASTString.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTString::ASTString( yy::location location ) 
    : ASTNode( location ) 
{ 
}

ASTString::ASTString( std::string str)
    : _str(std::move( str ))
{
}
ASTString::ASTString( yy::location location, std::string str)
    : ASTNode( location )
    , _str(std::move( str ))
{
}

void ASTString::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve