#pragma once

#include "ASTVisitor.hh"

namespace sieve
{

class ASTTraceVisitor : public ASTVisitor {
public:
    void walk( ASTSieve * ) override;

    void visit( ASTBlock* ) override;
    void visit( ASTBoolean* ) override;
    void visit( ASTBranch* ) override;
    void visit( ASTCommand* ) override;
    void visit( ASTCondition* ) override;
    void visit( ASTNoOp* ) override;
    void visit( ASTNumeric* ) override;
    void visit( ASTRequire* ) override;
    void visit( ASTSieve* ) override;
    void visit( ASTString* ) override;
    void visit( ASTStringList* ) override;
    void visit( ASTTag* ) override;
    void visit( ASTTest* ) override;
    void visit( ASTTestList* ) override;

private:
    void _traverse_tree(sieve::ASTNode *node, int indent_level);
};

} // namespace sieve
