#ifndef __ASTCOMMAND_HH__
#define __ASTCOMMAND_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTCommand : public ASTNode {
public:
    ASTCommand() : ASTNode() {}    
    ASTCommand(yy::location location);
    ASTCommand(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() { return this->_name; }
    
private:
    std::string _name;
};

} // namespace sieve

#endif
