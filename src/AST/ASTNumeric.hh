#ifndef __ASTNUMERIC_HH__
#define __ASTNUMERIC_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTNumeric : public ASTNode {
public:
    ASTNumeric() : ASTNode() {}    
    ASTNumeric(yy::location location);
    ASTNumeric(int number);
    ASTNumeric(yy::location location, int number);
    
    void accept(ASTVisitor &visitor);
    
    int value() const { return this->_number; }

    template<class T>
    ASTNode *find(const T& value) const {
        for (auto i = this->children().begin(); i != this->children().end(); ++i) {
            const T* child;

            if (*i == NULL)
                return NULL;

            child = dynamic_cast<T*>(*i);

            if (child == NULL)
                continue;

            if (child->value() == value.value())
                return *i;
        }

        return NULL;
    }
    
private:
    int _number;
};

} // namespace sieve

#endif
