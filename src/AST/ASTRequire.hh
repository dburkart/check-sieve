#ifndef __ASTREQUIRE_HH__
#define __ASTREQUIRE_HH__


#include "ASTCommand.hh"

namespace sieve
{

class ASTVisitor;

class ASTRequire : public ASTCommand {
public:
    ASTRequire() : ASTCommand() {}    
    ASTRequire(yy::location location);
    
    void accept(ASTVisitor &visitor);
    
    std::string value() const { return "ASTRequire"; }

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
