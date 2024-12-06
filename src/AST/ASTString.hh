#pragma once

#include <string>

#include "ASTNode.hh"

namespace sieve
{

class ASTVisitor;

class ASTString : public ASTNode {
public:
    ASTString() : ASTNode() {}    
    explicit ASTString(yy::location location);
    explicit ASTString(std::string str);
    ASTString(yy::location location, std::string str);
    
    void accept(ASTVisitor &visitor) override;
    
    [[nodiscard]] std::string_view value() const { return std::string_view{ this->_str }; }

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
    std::string _str;
};

} // namespace sieve
