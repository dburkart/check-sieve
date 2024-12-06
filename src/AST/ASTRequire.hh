#pragma once

#include "ASTCommand.hh"

namespace sieve
{

class ASTVisitor;

class ASTRequire : public ASTCommand {
public:
    ASTRequire() : ASTCommand() {}    
    explicit ASTRequire(yy::location location);
    
    void accept(ASTVisitor &visitor) override;
    
    [[nodiscard]] std::string value() const { return "ASTRequire"; }

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
};

} // namespace sieve
