#ifndef __AST_VERIFICATION_VISITOR_H__
#define __AST_VERIFICATION_VISITOR_H__

#include <map>
#include <string>

#include "checksieve.h"
#include "ASTVisitor.hh"
#include "Validation/Command.hh"

namespace sieve
{

class ASTVerificationVisitor : public ASTVisitor {
public:
    ASTVerificationVisitor();
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

    parse_result result() { return _verification_result; }

private:
    void _init();
    void _traverse_tree(sieve::ASTNode *node);

    parse_result _verification_result;

    std::map<std::string, bool> _command_map;
    std::map<std::string, bool> _test_map;
    std::map<std::string, bool> _tag_map;
    
    Command _command;
};

} // namespace sieve

#endif
