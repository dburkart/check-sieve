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
