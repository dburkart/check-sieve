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
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }
    

private:
    std::string _description;
};

} // namespace sieve

#endif
