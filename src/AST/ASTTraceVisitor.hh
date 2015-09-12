#ifndef __AST_TRACE_VISITOR_H__
#define __AST_TRACE_VISITOR_H__

#include "ASTVisitor.hh"

namespace sieve
{

class ASTTraceVisitor : public ASTVisitor {
public:
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
};

} // namespace sieve

#endif
