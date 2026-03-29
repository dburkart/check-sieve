#pragma once

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTNumeric : public ASTNode {
public:
    ASTNumeric() : ASTNode() {}
    ASTNumeric(yy::location location);
    ASTNumeric(long number);
    ASTNumeric(yy::location location, long number);

    void accept(ASTVisitor &visitor);

    long value() const { return this->_number; }

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
    long _number;
};

} // namespace sieve

