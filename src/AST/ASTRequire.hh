#ifndef __ASTREQUIRE_HH__
#define __ASTREQUIRE_HH__


#include "ASTCommand.hh"

namespace sieve
{

class ASTVisitor;

class ASTRequire : public ASTCommand {
public:
    ASTRequire() : ASTCommand() {}    
    ASTRequire(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTRequire"; }
    
private:
};

} // namespace sieve

#endif
