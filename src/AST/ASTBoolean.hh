#pragma once

#include "ASTTest.hh"

namespace sieve
{

class ASTVisitor;

class ASTBoolean : public ASTTest {
public:
    ASTBoolean() : ASTTest() {}    
    ASTBoolean(yy::location location);
    ASTBoolean(bool val);
    ASTBoolean(yy::location location, bool val);
    
    void accept(ASTVisitor &visitor);
    
    bool value() const { return this->_val; }

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
    bool _val;
};

} // namespace sieve

