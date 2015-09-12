#include <iostream>
#include <vector>

#include "ASTVerificationVisitor.hh"

namespace sieve
{

void ASTVerificationVisitor::walk( ASTSieve *root ) {
    this->_traverse_tree( root, 0 );
}

void ASTVerificationVisitor::_traverse_tree( sieve::ASTNode *node, int indent_level ) {

}

void ASTVerificationVisitor::visit( ASTBlock* node ) {

}

void ASTVerificationVisitor::visit( ASTBoolean* node ) {

}

void ASTVerificationVisitor::visit( ASTBranch* node ) {

}

void ASTVerificationVisitor::visit( ASTCommand* node ) {

}

void ASTVerificationVisitor::visit( ASTCondition* node ) {

}

void ASTVerificationVisitor::visit( ASTNumeric* node ) {

}

void ASTVerificationVisitor::visit( ASTRequire* node ) {

}

void ASTVerificationVisitor::visit( ASTSieve* node ) {

}

void ASTVerificationVisitor::visit( ASTString* node ) {

}

void ASTVerificationVisitor::visit( ASTTag* node ) {

}

void ASTVerificationVisitor::visit( ASTTest* node ) {

}

} // namespace sieve

