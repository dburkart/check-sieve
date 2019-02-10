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

    int length() const {
        int length = 0;
        for (std::vector<ASTNode *>::const_iterator it = this->children().begin(); it != this->children().end(); ++it) {
            length += 1;
        }
        return length;
    }
    
private:
};

} // namespace sieve

#endif
