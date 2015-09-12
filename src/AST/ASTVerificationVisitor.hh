#ifndef __AST_VERIFICATION_VISITOR_H__
#define __AST_VERIFICATION_VISITOR_H__

#include "checksieve.h"
#include "ASTVisitor.hh"

namespace sieve
{

class ASTVerificationVisitor : public ASTVisitor {
public:
    ASTVerificationVisitor() : _verification_result() {}
    void walk( ASTSieve * );

    void visit( ASTBlock* );
    void visit( ASTBoolean* );
    void visit( ASTBranch* );
    void visit( ASTCommand* );
    void visit( ASTCondition* );
    void visit( ASTNumeric* );
    void visit( ASTRequire* );
    void visit( ASTSieve* );
    void visit( ASTString* );
    void visit( ASTTag* );
    void visit( ASTTest* );

private:
    void _traverse_tree(sieve::ASTNode *node, int indent_level);

    parse_result _verification_result;
};

} // namespace sieve

#endif
