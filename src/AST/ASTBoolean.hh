#ifndef __ASTBOOLEAN_HH__
#define __ASTBOOLEAN_HH__


#include "ASTTest.hh"

namespace sieve
{

class ASTVisitor;

class ASTBoolean : public ASTTest {
public:
    ASTBoolean() : ASTTest() {}    
    ASTBoolean(yy::location location);
    ASTBoolean(bool val);
    ASTBoolean(yy::location location, bool val);
    
    void accept(ASTVisitor &visitor);
    
    bool value() const { return this->_val; }

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
    bool _val;
};

} // namespace sieve

#endif
