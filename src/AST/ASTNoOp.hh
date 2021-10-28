#pragma once

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTNoOp : public ASTNode {
public:
    ASTNoOp() : ASTNode() {}    
    ASTNoOp(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTNoOp"; }

    template<class T>
    std::vector<ASTNode *>::const_iterator find(const T& value) const {
        for (std::vector<ASTNode *>::const_iterator it = this->children().begin(); it != this->children().end(); ++it) {
            const T* child = dynamic_cast<T*>(*it);

            if (child == NULL)
                continue;

            if (child->value() == value.value())
                return it;
        }

        return this->children().end();
    }
    
private:
};

} // namespace sieve
