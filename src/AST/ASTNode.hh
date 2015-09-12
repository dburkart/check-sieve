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
    ASTNode() {}
    ASTNode(yy::location location) {
        this->_location = location;
        this->_parent = NULL;
    }

    virtual void accept(ASTVisitor& visitor) =0;

    ASTNode *parent() { return this->_parent; }
    ASTNode *parent( ASTNode *parent ) { this->_parent = parent; return this->_parent; }
    
    std::vector<ASTNode *> children() { return this->_children; }
    
    void push(ASTNode *child) { this->_children.push_back(child); } 
    void push(std::vector<ASTNode *> children) { 
        this->_children.insert(this->_children.end(), children.begin(), children.end());
    }

private:
    std::vector<ASTNode *> _children;
    yy::location _location;
    ASTNode *_parent;
};

} // namespace sieve

#endif
