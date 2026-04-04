#pragma once

#include <ctime>
#include <map>
#include <optional>
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
        std::string bodyTransform;  // empty = not explicitly set (body test defaults to :text)
        std::string relationalOp;   // RFC 5231: "gt", "lt", "ge", "le", "eq", "ne"
        std::string comparator;     // RFC 5231: "i;ascii-casemap", "i;ascii-numeric", etc.
        std::vector<std::string> headerNames;
        std::vector<std::string> values;
        std::vector<std::string> contentTypes;
        long numericValue = 0;
        bool hasNumeric = false;
        int  indexField  = 0;       // RFC 5260: 0 = not set; 1..N = :index value
        bool indexLast   = false;   // RFC 5260: true if :last was specified
        std::string zone;           // RFC 5260: "" = UTC/local; "original" = :originalzone; "+hhmm"/"-hhmm" = :zone
    };

    void _simulate(ASTNode *node);
    void _simulateBlock(ASTNode *node);
    bool _evaluateTest(ASTNode *node, bool quiet = false);
    TestArgs _collectTestArgs(ASTNode *node);
    std::string _describeTest(ASTNode *node);

    static bool _matchString(const std::string &value, const std::string &pattern, const std::string &matchType,
                             std::vector<std::string> *captures = nullptr);
    static bool _relationalCompare(const std::string &left, const std::string &right,
                                   const std::string &op, const std::string &comparator);
    static bool _globMatch(const std::string &str, const std::string &pattern,
                            std::vector<std::string> *captures = nullptr);
    static bool _contentTypeMatches(const std::string &partType, const std::vector<std::string> &patterns);
    static std::optional<std::string> _extractAddressPart(const std::string &headerValue, const std::string &partTag);
    static std::vector<std::string> _applyIndex(const std::vector<std::string> &values, int indexField, bool indexLast);
    static bool _parseRFC2822Date(const std::string &headerValue, struct tm &tmOut, int &offsetMinutes);
    static std::string _extractDatePart(const struct tm &utcTm, const std::string &part, int offsetMinutes);
    static std::string _extractAddress(const std::string &headerValue);
    static std::vector<std::string> _getStrings(ASTNode *node);

    std::string _expandVariables(const std::string &s) const;
    std::string _applyModifiers(const std::string &value, const std::vector<std::string> &modifiers) const;

    static std::vector<std::string> _parseFlags(const std::vector<std::string> &flagStrings);
    std::string _getFlagSet(const std::string &varName) const;
    void _setFlagSet(const std::string &varName, const std::string &flags);

    const EmailMessage &_email;
    std::string _sieveFile;
    std::string _emailFile;
    bool _stopped = false;
    bool _deliveryActionTaken = false;
    std::vector<std::string> _actions;
    std::map<std::string, std::string> _variables;
    std::vector<std::string> _matchVars = std::vector<std::string>(10, "");
    std::string _internalFlags;   // RFC 5232: implicit flag variable (space-separated)
    std::map<std::string, std::string> _environmentItems;  // RFC 5183
    std::time_t _scriptTime = 0;  // RFC 5260: captured once for all currentdate tests
};

} // namespace sieve
