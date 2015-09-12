#ifndef __AST_NODE_HH__
#define __AST_NODE_HH__

#include <vector>
#include <typeinfo>

#include "checksieve.h"

namespace sieve
{

class ASTVisitor;

class ASTNode {
public:
    ASTNode(yy::location location) {
        this->_location = location;
    }

    virtual void accept(ASTVisitor& visitor) =0;
    
private:
    std::vector<ASTNode *> _children;
    yy::location _location;
};

} // namespace sieve

#endif
