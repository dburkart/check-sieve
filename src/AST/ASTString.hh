#ifndef __ASTSTRING_HH__
#define __ASTSTRING_HH__

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTString : public ASTNode {
public:
    ASTString() : ASTNode() {}    
    ASTString(yy::location location);
    ASTString(std::string str);
    ASTString(yy::location location, std::string str);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return this->_str; }

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
    std::string _str;
};

} // namespace sieve

#endif
