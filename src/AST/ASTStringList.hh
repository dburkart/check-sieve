#ifndef __ASTSTRINGLIST_HH__
#define __ASTSTRINGLIST_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTStringList : public ASTNode {
public:
    ASTStringList() : ASTNode() {}    
    ASTStringList(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTStringList"; }

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
