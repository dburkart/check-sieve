#ifndef __ASTREQUIRE_HH__
#define __ASTREQUIRE_HH__

#include <string>

#include "ASTCommand.hh"

namespace sieve
{

class ASTVisitor;

class ASTRequire : public ASTCommand {
public:
    ASTRequire() : ASTCommand() {}    
    ASTRequire(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    
private:
};

} // namespace sieve

#endif
