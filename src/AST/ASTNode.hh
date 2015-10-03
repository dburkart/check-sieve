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
    ASTNode() : _children() {}
    ASTNode(const yy::location &location)
        : _children()
        , _location(location.begin, location.end) {}

    virtual void accept(ASTVisitor& visitor) =0;
    
    std::vector<ASTNode *> children() const { return this->_children; }
    
    void push(ASTNode *child) { this->_children.push_back(child); } 
    void push(std::vector<ASTNode *> children) { 
        this->_children.insert(this->_children.end(), children.begin(), children.end());
    }

    yy::location location() { return this->_location; }

private:
    std::vector<ASTNode *> _children;
    yy::location _location;
};

} // namespace sieve

#endif
