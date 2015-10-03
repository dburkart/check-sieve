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
    
    int value() const { return this->_number; }
    
private:
    int _number;
};

} // namespace sieve

#endif
