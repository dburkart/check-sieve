#ifndef __ASTCOMMAND_HH__
#define __ASTCOMMAND_HH__


#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTCommand : public ASTNode {
public:
    ASTCommand() : ASTNode() {}    
    ASTCommand(yy::location location);
    ASTCommand(std::string name);
    ASTCommand(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return this->_name; }

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
    std::string _name;
};

} // namespace sieve

#endif
