#include <iostream>
#include <vector>

#include "ASTVerificationVisitor.hh"

namespace sieve
{

ASTVerificationVisitor::ASTVerificationVisitor()
    : _verification_result() {
    _init();
}

void ASTVerificationVisitor::walk( ASTSieve *root ) {
    this->_traverse_tree( root );
}

void ASTVerificationVisitor::_traverse_tree( sieve::ASTNode *node ) {
    node->accept(*this);

    if (result().status)
        return;

    std::vector<sieve::ASTNode *> children = node->children();
    for (std::vector<sieve::ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        sieve::ASTNode *child = *it;
        _traverse_tree(child);
    }
}

void ASTVerificationVisitor::visit( ASTBlock* node ) {

}

void ASTVerificationVisitor::visit( ASTBoolean* node ) {

}

void ASTVerificationVisitor::visit( ASTBranch* node ) {

}

void ASTVerificationVisitor::visit( ASTCommand* node ) {
    if (!_command_map[node->value()]) {
        _verification_result.set_error(node->location(), "Unrecognized command \"" + node->value() + "\".");
    }
}

void ASTVerificationVisitor::visit( ASTCondition* node ) {

}

void ASTVerificationVisitor::visit( ASTNoOp* ) {
    // No Op
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
    if (!_test_map[node->value()]) {
        _verification_result.set_error(node->location(), "Unrecognized test \"" + node->value() + "\".");
    }
}

//-- Private methods
void ASTVerificationVisitor::_init() {
    _command_map["keep"] = 1;
    _command_map["discard"] = 1;
    _command_map["redirect"] = 1;
    _command_map["reject"] = 1;
    _command_map["fileinto"] = 1;
    _command_map["stop"] = 1;

    // RFC 5232
    _command_map["setflag"] = 1;
    _command_map["addflag"] = 1;
    _command_map["removeflag"] = 1;

    // RFC 5229
    _command_map["set"] = 1;

    // RFC 5230
    _command_map["vacation"] = 1;

    // RFC 5429
    _command_map["ereject"] = 1;

    // RFC 5703
    _command_map["foreverypart"] = 1;
    _command_map["break"] = 1;
    _command_map["extracttext"] = 1;
    _command_map["replace"] = 1;
    _command_map["enclose"] = 1;

    // RFC 6609
    _command_map["include"] = 1;
    _command_map["return"] = 1;

    _test_map["allof"] = 1;
    _test_map["anyof"] = 1;
    _test_map["address"] = 1;
    _test_map["envelope"] = 1;
    _test_map["header"] = 1;
    _test_map["size"] = 1;
    _test_map["not"] = 1;
    _test_map["exists"] = 1;

    // RFC 5173
    _test_map["body"] = 1;

    // RFC 5232
    _test_map["hasflag"] = 1;

    // RFC 5229
    _test_map["string"] = 1;

    // RFC 5260
    _test_map["date"] = 1;
    _test_map["currentdate"] = 1;
}

} // namespace sieve

