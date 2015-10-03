#ifndef __ASTBOOLEAN_HH__
#define __ASTBOOLEAN_HH__

#include <string>

#include "ASTTest.hh"

namespace sieve
{

class ASTVisitor;

class ASTBoolean : public ASTTest {
public:
    ASTBoolean() : ASTTest() {}    
    ASTBoolean(yy::location location);
    ASTBoolean(yy::location location, bool val);
    
    void accept(ASTVisitor &visitor);
    
    bool value() const { return this->_val; }
    
private:
    bool _val;
};

} // namespace sieve

#endif
