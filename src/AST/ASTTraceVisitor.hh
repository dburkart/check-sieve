#pragma once

#include "ASTVisitor.hh"

namespace sieve
{

class ASTTraceVisitor : public ASTVisitor {
public:
    void walk( ASTSieve * );

    void visit( ASTBlock* );
    void visit( ASTBoolean* );
    void visit( ASTBranch* );
    void visit( ASTCommand* );
    void visit( ASTCondition* );
    void visit( ASTNoOp* );
    void visit( ASTNumeric* );
    void visit( ASTRequire* );
    void visit( ASTSieve* );
    void visit( ASTString* );
    void visit( ASTStringList* );
    void visit( ASTTag* );
    void visit( ASTTest* );

private:
    void _traverse_tree(sieve::ASTNode *node, int indent_level);
};

} // namespace sieve
