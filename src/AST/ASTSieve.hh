#ifndef __ASTSIEVE_HH__
#define __ASTSIEVE_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTSieve : public ASTNode {
public:
    ASTSieve() : ASTNode() {}    
    ASTSieve(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTSieve"; }

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
};

} // namespace sieve

#endif
