#ifndef __ASTNUMERIC_HH__
#define __ASTNUMERIC_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTNumeric : public ASTNode {
public:
    ASTNumeric() : ASTNode() {}    
    ASTNumeric(yy::location location);
    ASTNumeric(yy::location location, int number);
    
    void accept(ASTVisitor &visitor);
    
    std::string description() { return this->_description; }
    std::string description( std::string description) { this->_description = description; return this->_description; }
    
    int value() { return this->_number; }

private:
    std::string _description;
    int _number;
};

} // namespace sieve

#endif
