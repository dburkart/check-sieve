#include "ASTTest.hh"
#include "ASTVisitor.hh"

namespace sieve
{

ASTTest::ASTTest( yy::location location ) 
    : ASTCommand( location ) 
{ 
}

ASTTest::ASTTest( yy::location location, std::string name)
    : ASTCommand( location )
    , _name( name )
{
}

void ASTTest::accept( ASTVisitor &visitor ) {
    visitor.visit(this);
}

} // namespace sieve