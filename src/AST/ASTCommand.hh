#pragma once

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTCommand : public ASTNode {
public:
    ASTCommand() : ASTNode() {}    
    explicit ASTCommand(yy::location location);
    explicit ASTCommand(std::string name);
    ASTCommand(yy::location location, std::string name);
    
    void accept(ASTVisitor &visitor) override;
    
    [[nodiscard]] std::string value() const { return this->_name; }

    template<class T>
    std::vector<ASTNode *>::const_iterator find(const T& value) const {
        for (auto it = this->children().begin(); it != this->children().end(); ++it) {
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

