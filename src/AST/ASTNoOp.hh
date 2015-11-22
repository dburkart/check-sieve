#ifndef __ASTNOOP_HH__
#define __ASTNOOP_HH__


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
    ASTNode *find(const T& value) const {
        for (auto i = this->children().begin(); i != this->children().end(); ++i) {
            const T* child = dynamic_cast<T*>(*i);

            if (child == NULL)
                continue;

            if (child->value() == value.value())
                return *i;
        }

        return NULL;
    }
    
private:
};

} // namespace sieve

#endif
