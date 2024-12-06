#pragma once

#include <utility>

#include "ASTNode.hh"

namespace sieve {

template <class T>
bool nodeIsType(const ASTNode *node) {
    const T *t_node = dynamic_cast<const T*>(node);
    return t_node != NULL;
}

class ValidationResult {
public:
    explicit ValidationResult(bool r) : _result(r), _hint_as_error(false) { };
    ValidationResult(bool r, std::string h) : _result(r), _hint(std::move(h)), _hint_as_error(false) { };
    ValidationResult(bool r, std::string h, bool asErr) : _result(r), _hint(std::move(h)), _hint_as_error(asErr) { };
    ~ValidationResult() = default;

    const bool result() { return this->_result; }
    const std::string hint() { return this->_hint; }
    const bool hint_as_error() { return this->_hint_as_error; }

private:
    const bool _result;
    const std::string _hint;
    const bool _hint_as_error;
};

class Validator {
public:
    Validator() = default;
    ~Validator() = default;

    void set_visitor( const ASTVisitor *v ) { _visitor = v; }

    virtual ValidationResult validate( const ASTNode * ) =0;
    virtual std::string usage( const ASTNode * ) =0;
    
    const ASTVisitor *visitor() { return _visitor; }

private:
    const ASTVisitor *_visitor;
};

}
