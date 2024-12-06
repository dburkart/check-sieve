#pragma once

#include <map>
#include <string>

#include "ASTString.hh"
#include "checksieve.h"
#include "ASTVisitor.hh"
#include "Validation/Command.hh"
#include "Validation/Tag.hh"
#include "Validation/Test.hh"

namespace sieve
{

class ASTVerificationVisitor : public ASTVisitor {
public:
    ASTVerificationVisitor( struct parse_options options);
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
    void visit( ASTTestList* );

    parse_result result() { return _verification_result; }
    
    bool requires_capability( std::string capability ) { return _capability_map[capability]; }
    bool has_required( std::string require ) {
        return requires_capability(require) || (
            _required_capabilities != nullptr &&
            _required_capabilities->find(ASTString(require)) != _required_capabilities->children().end());
    }

private:
    void _init();
    void _traverse_tree(sieve::ASTNode *node);
    void _enable_capability(std::string_view capability);

    struct parse_options _options;
    parse_result _verification_result;
    
    std::map<std::string_view, bool> _capability_map;
    std::map<std::string_view, bool> _command_map;
    std::map<std::string_view, bool> _test_map;
    std::map<std::string_view, bool> _tag_map;
    std::map<std::string_view, std::string> _require_lookup;
    
    // Validators
    Command _command;
    Tag _tag;
    Test _test;

    // Other state
    ASTStringList *_required_capabilities;          // TODO: This should really be a vector or map
};

} // namespace sieve
