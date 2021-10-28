#pragma once

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
        : _parent(NULL)
        , _children()
        , _location(location.begin, location.end) {}

    virtual void accept(ASTVisitor& visitor) =0;
    
    const std::vector<ASTNode *> &children() const { return this->_children; }
    const ASTNode *nextChild(const ASTNode *child) const {
        const ASTNode *next = NULL;
        std::vector<sieve::ASTNode *> children = _children;
        for (std::vector<ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
            if (child == *it) {
                ++it;
                if (it == children.end())
                    break;
                next = *it;
            }
        }
        return next;
    };
    
    const ASTNode *parent() const { return _parent; }
    void setParent(ASTNode *parent) { _parent = parent; }

    void push(ASTNode *child) { child->setParent(this); this->_children.push_back(child); }
    void push(std::vector<ASTNode *> children) { 
        for (std::vector<ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
            (*it)->setParent(this);
        }
        this->_children.insert(this->_children.end(), children.begin(), children.end());
    }

    yy::location location() const { return this->_location; }

private:
    ASTNode *_parent;
    std::vector<ASTNode *> _children;
    yy::location _location;
};

} // namespace sieve

