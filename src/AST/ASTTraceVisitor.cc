#include <iostream>
#include <vector>

#include "ASTTraceVisitor.hh"

namespace sieve
{
    
void ASTTraceVisitor::walk( ASTSieve *root ) {
    this->_traverse_tree( root, 0 );
}

void ASTTraceVisitor::_traverse_tree( sieve::ASTNode *node, int indent_level ) {
    for (int i = 0; i < indent_level; i++) {
        std::cout << "    ";
    }

    node->accept(*this);

    std::vector<sieve::ASTNode *> children = node->children();
    for (auto child : children) {
        _traverse_tree(child, indent_level + 1);
    }
}

void ASTTraceVisitor::visit( ASTBlock* node ) {
    std::cout << "Block" << std::endl;
}

void ASTTraceVisitor::visit( ASTBoolean* node ) {
    if (node->value()) {
        std::cout << "True" << std::endl;
    } else {
        std::cout << "False" << std::endl;
    }
}

void ASTTraceVisitor::visit( ASTBranch* node ) {
    std::cout << "Branch" << std::endl;
}

void ASTTraceVisitor::visit( ASTCommand* node ) {
    std::cout << "Command (" << node->value() << ")" << std::endl;
}

void ASTTraceVisitor::visit( ASTCondition* node ) {
    std::cout << "Condition" << std::endl;
}

void ASTTraceVisitor::visit( ASTNoOp* ) {
    // No Op
}

void ASTTraceVisitor::visit( ASTNumeric* node ) {
    std::cout << "Numeric (" << node->value() << ")" << std::endl;
}

void ASTTraceVisitor::visit( ASTRequire* node ) {
    std::cout << "Require" << std::endl;
}

void ASTTraceVisitor::visit( ASTSieve* node ) {
    std::cout << "Mail Sieve" << std::endl;
}

void ASTTraceVisitor::visit( ASTString* node ) {
    const auto slice = node->value().substr(0, 10);
    if ( slice != node->value() ) {
        std::cout << "String (\"" << slice << "..." <<  "\")" << std::endl;
    } else {
        std::cout << "String (\"" << slice <<  "\")" << std::endl;
    }
}

void ASTTraceVisitor::visit( ASTStringList* node ) {
    std::cout << "String List" << std::endl;
}

void ASTTraceVisitor::visit( ASTTag* node ) {
    std::cout << "Tag (" << node->value() << ")" << std::endl;
}

void ASTTraceVisitor::visit( ASTTest* node ) {
    std::cout << "Test (" << node->value() << ")" << std::endl;
}

void ASTTraceVisitor::visit( ASTTestList* node ) {
    std::cout << "Test-List" << std::endl;
}

} // namespace sieve

