#ifndef __ASTTEST_HH__
#define __ASTTEST_HH__


#include "ASTCommand.hh"

namespace sieve
{

class ASTVisitor;

class ASTTest : public ASTCommand {
public:
    ASTTest() : ASTCommand() {}    
    ASTTest(yy::location location);
    ASTTest(std::string name);
    ASTTest(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return this->_name; }

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
    std::string _name;
};

} // namespace sieve

#endif
