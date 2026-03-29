#pragma once

#include <string>
#include <vector>

#include "ASTVisitor.hh"
#include "EmailMessage.hh"

namespace sieve
{

class ASTSimulationVisitor : public ASTVisitor {
public:
    ASTSimulationVisitor(const EmailMessage &email, const std::string &sieveFile, const std::string &emailFile);
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
    struct TestArgs {
        std::string matchType = ":is";
        std::string addressPart = ":all";
        std::string sizeComparator;
        std::vector<std::string> headerNames;
        std::vector<std::string> values;
        long numericValue = 0;
        bool hasNumeric = false;
    };

    void _simulate(ASTNode *node);
    void _simulateBlock(ASTNode *node);
    bool _evaluateTest(ASTNode *node, bool quiet = false);
    TestArgs _collectTestArgs(ASTNode *node);
    std::string _describeTest(ASTNode *node);

    static bool _matchString(const std::string &value, const std::string &pattern, const std::string &matchType);
    static bool _globMatch(const std::string &str, const std::string &pattern);
    static std::string _extractAddressPart(const std::string &headerValue, const std::string &partTag);
    static std::string _extractAddress(const std::string &headerValue);
    static std::vector<std::string> _getStrings(ASTNode *node);

    const EmailMessage &_email;
    std::string _sieveFile;
    std::string _emailFile;
    bool _stopped = false;
    bool _deliveryActionTaken = false;
    std::vector<std::string> _actions;
};

} // namespace sieve
