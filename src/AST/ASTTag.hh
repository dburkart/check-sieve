#ifndef __ASTTAG_HH__
#define __ASTTAG_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTTag : public ASTNode {
public:
    ASTTag() : ASTNode() {}    
    ASTTag(yy::location location);
    ASTTag(std::string name);
    ASTTag(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return this->_name; }

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
    std::string _name;
};

} // namespace sieve

#endif
