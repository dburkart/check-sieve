#ifndef __AST_VISITOR_H__
#define __AST_VISITOR_H__

#include "ASTBoolean.hh"
#include "ASTBranch.hh"
#include "ASTCommand.hh"
#include "ASTCondition.hh"
#include "ASTNumeric.hh"
#include "ASTString.hh"
#include "ASTTag.hh"

namespace sieve
{

class ASTVisitor {
public:
    virtual void visit( ASTBoolean* ) =0;
    virtual void visit( ASTBranch* ) =0;
    virtual void visit( ASTCommand* ) =0;
    virtual void visit( ASTCondition* ) =0;
    virtual void visit( ASTNumeric* ) =0;
    virtual void visit( ASTString* ) =0;
    virtual void visit( ASTTag* ) =0;
};

} // namespace sieve

#endif
