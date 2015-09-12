#include <iostream>

#include "ASTTraceVisitor.hh"

namespace sieve
{
    
void ASTTraceVisitor::visit( ASTBlock* node ) {
    std::cout << "Block of commands" << std::endl;
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
    std::string slice = node->value().substr(0, 10);
    if (slice != node->value())
        slice += "...";
    std::cout << "String (\"" << slice <<  "\")" << std::endl;
}

void ASTTraceVisitor::visit( ASTTag* node ) {
    std::cout << "Tag (" << node->value() << ")" << std::endl;
}

void ASTTraceVisitor::visit( ASTTest* node ) {
    std::cout << "Test (" << node->value() << ")" << std::endl;
}

} // namespace sieve

