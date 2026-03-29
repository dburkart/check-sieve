#include <algorithm>
#include <iostream>
#include <sstream>

#include "ASTSimulationVisitor.hh"

namespace sieve
{

ASTSimulationVisitor::ASTSimulationVisitor(const EmailMessage &email, const std::string &sieveFile, const std::string &emailFile)
    : _email(email)
    , _sieveFile(sieveFile)
    , _emailFile(emailFile)
{
}

void ASTSimulationVisitor::walk( ASTSieve *root ) {
    std::cout << "Simulating \"" << _sieveFile << "\" with \"" << _emailFile << "\"" << std::endl;
    std::cout << "---" << std::endl;

    for (auto *child : root->children()) {
        if (_stopped) break;
        _simulate(child);
    }

    // If no actions were taken, implicit keep
    if (_actions.empty() && !_stopped) {
        std::cout << "ACTION: keep (implicit)" << std::endl;
        _actions.push_back("keep (implicit)");
    }

    std::cout << "---" << std::endl;
    std::cout << "Actions:";
    for (size_t i = 0; i < _actions.size(); i++) {
        if (i > 0) std::cout << ",";
        std::cout << " " << _actions[i];
    }
    std::cout << std::endl;
}

void ASTSimulationVisitor::_simulate(ASTNode *node) {
    if (_stopped) return;

    if (auto *branch = dynamic_cast<ASTBranch*>(node)) {
        // Branch children alternate: Condition, Block, [Condition, Block, ...], [Block(else)]
        const auto &children = branch->children();
        bool branchTaken = false;

        size_t i = 0;
        while (i < children.size() && !branchTaken) {
            auto *condition = dynamic_cast<ASTCondition*>(children[i]);
            if (condition) {
                // Evaluate the condition's test (first child of condition)
                bool result = false;
                if (!condition->children().empty()) {
                    result = _evaluateTest(condition->children()[0]);
                }

                if (result && i + 1 < children.size()) {
                    _simulateBlock(children[i + 1]);
                    branchTaken = true;
                }
                i += 2;
            } else {
                // This is an else block (no preceding condition)
                _simulateBlock(children[i]);
                branchTaken = true;
                i++;
            }
        }
        return;
    }

    if (dynamic_cast<ASTRequire*>(node)) {
        return;
    }

    if (auto *command = dynamic_cast<ASTCommand*>(node)) {
        std::string name = command->value();

        if (name == "keep" || name == "discard" || name == "stop" ||
            name == "fileinto" || name == "redirect" || name == "reject" ||
            name == "ereject") {

            std::string actionDesc = name;

            // Extract argument for commands that take one
            if (name == "fileinto" || name == "redirect" || name == "reject" || name == "ereject") {
                for (auto *child : command->children()) {
                    auto *str = dynamic_cast<ASTString*>(child);
                    if (str) {
                        actionDesc += " \"" + std::string(str->value()) + "\"";
                        break;
                    }
                }
            }

            std::cout << "ACTION: " << actionDesc << std::endl;
            _actions.push_back(actionDesc);

            if (name == "stop") {
                _stopped = true;
            }
            return;
        }

        // For other commands (set, addflag, etc.), just note them
        {
            std::cout << "ACTION: " << name;
            for (auto *child : command->children()) {
                if (auto *str = dynamic_cast<ASTString*>(child)) {
                    std::cout << " \"" << str->value() << "\"";
                } else if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    std::cout << " " << tag->value();
                }
            }
            std::cout << std::endl;
            _actions.push_back(name);
        }
        return;
    }

    // For any other node type, try to recurse into children
    for (auto *child : node->children()) {
        if (_stopped) break;
        _simulate(child);
    }
}

void ASTSimulationVisitor::_simulateBlock(ASTNode *node) {
    if (_stopped) return;
    for (auto *child : node->children()) {
        if (_stopped) break;
        _simulate(child);
    }
}

bool ASTSimulationVisitor::_evaluateTest(ASTNode *node, bool quiet) {
    // Boolean literals
    if (auto *boolean = dynamic_cast<ASTBoolean*>(node)) {
        return boolean->value();
    }

    auto *test = dynamic_cast<ASTTest*>(node);
    if (!test) {
        // TestList shouldn't appear here directly, but handle it
        if (auto *testList = dynamic_cast<ASTTestList*>(node)) {
            // Treat bare test list as allof
            for (auto *child : testList->children()) {
                if (!_evaluateTest(child, quiet)) return false;
            }
            return true;
        }
        return false;
    }

    std::string testName = test->value();

    if (testName == "allof") {
        for (auto *child : test->children()) {
            if (auto *tl = dynamic_cast<ASTTestList*>(child)) {
                for (auto *subtest : tl->children()) {
                    if (!_evaluateTest(subtest, true)) return false;
                }
                if (!quiet) {
                    std::cout << "MATCH: " << _describeTest(test) << std::endl;
                }
                return true;
            }
            if (dynamic_cast<ASTTest*>(child) || dynamic_cast<ASTBoolean*>(child)) {
                if (!_evaluateTest(child, true)) return false;
            }
        }
        if (!quiet) {
            std::cout << "MATCH: " << _describeTest(test) << std::endl;
        }
        return true;
    }

    if (testName == "anyof") {
        for (auto *child : test->children()) {
            if (auto *tl = dynamic_cast<ASTTestList*>(child)) {
                for (auto *subtest : tl->children()) {
                    if (_evaluateTest(subtest, true)) {
                        if (!quiet) {
                            std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        }
                        return true;
                    }
                }
                return false;
            }
            if (dynamic_cast<ASTTest*>(child) || dynamic_cast<ASTBoolean*>(child)) {
                if (_evaluateTest(child, true)) {
                    if (!quiet) {
                        std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    if (testName == "not") {
        for (auto *child : test->children()) {
            if (dynamic_cast<ASTTest*>(child) || dynamic_cast<ASTBoolean*>(child)) {
                bool result = !_evaluateTest(child, true);
                if (result && !quiet) {
                    std::cout << "MATCH: " << _describeTest(test) << std::endl;
                }
                return result;
            }
        }
        return true;
    }

    if (testName == "header") {
        TestArgs args = _collectTestArgs(test);

        for (const auto &headerName : args.headerNames) {
            auto values = _email.header(headerName);
            for (const auto &hdrVal : values) {
                for (const auto &pattern : args.values) {
                    if (_matchString(hdrVal, pattern, args.matchType)) {
                        if (!quiet) {
                            std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    }

    if (testName == "address") {
        TestArgs args = _collectTestArgs(test);

        for (const auto &headerName : args.headerNames) {
            auto values = _email.header(headerName);
            for (const auto &hdrVal : values) {
                std::string addr = _extractAddressPart(hdrVal, args.addressPart);
                for (const auto &pattern : args.values) {
                    if (_matchString(addr, pattern, args.matchType)) {
                        if (!quiet) {
                            std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    }

    if (testName == "envelope") {
        std::cerr << "WARNING: envelope test approximated from message headers" << std::endl;
        TestArgs args = _collectTestArgs(test);

        for (const auto &envPart : args.headerNames) {
            std::string lowerPart = envPart;
            std::transform(lowerPart.begin(), lowerPart.end(), lowerPart.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            // Map envelope parts to headers
            std::string headerName;
            if (lowerPart == "from") headerName = "from";
            else if (lowerPart == "to") headerName = "to";
            else continue;

            auto hdrValues = _email.header(headerName);
            for (const auto &hdrVal : hdrValues) {
                std::string addr = _extractAddressPart(hdrVal, args.addressPart);
                for (const auto &pattern : args.values) {
                    if (_matchString(addr, pattern, args.matchType)) {
                        if (!quiet) {
                            std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        }
                        return true;
                    }
                }
            }
        }
        return false;
    }

    if (testName == "exists") {
        TestArgs args = _collectTestArgs(test);
        for (const auto &headerName : args.headerNames) {
            if (!_email.hasHeader(headerName)) {
                return false;
            }
        }
        if (!quiet) {
            std::cout << "MATCH: " << _describeTest(test) << std::endl;
        }
        return true;
    }

    if (testName == "size") {
        TestArgs args = _collectTestArgs(test);
        if (!args.hasNumeric) return false;

        bool result = false;
        if (args.sizeComparator == ":over") {
            result = static_cast<int>(_email.size()) > args.numericValue;
        } else if (args.sizeComparator == ":under") {
            result = static_cast<int>(_email.size()) < args.numericValue;
        }

        if (result && !quiet) {
            std::cout << "MATCH: " << _describeTest(test) << std::endl;
        }
        return result;
    }

    // Unsupported test
    std::cerr << "WARNING: Unsupported test \"" << testName << "\" -- assuming false" << std::endl;
    return false;
}

ASTSimulationVisitor::TestArgs ASTSimulationVisitor::_collectTestArgs(ASTNode *node) {
    TestArgs args;
    std::vector<ASTNode*> stringArgs;

    for (auto *child : node->children()) {
        if (auto *tag = dynamic_cast<ASTTag*>(child)) {
            std::string tagVal = tag->value();
            if (tagVal == ":is" || tagVal == ":contains" || tagVal == ":matches") {
                args.matchType = tagVal;
            } else if (tagVal == ":localpart" || tagVal == ":domain" || tagVal == ":all") {
                args.addressPart = tagVal;
            } else if (tagVal == ":over" || tagVal == ":under") {
                args.sizeComparator = tagVal;
            }
            // Ignore other tags (comparator, etc.)
        } else if (dynamic_cast<ASTString*>(child)) {
            stringArgs.push_back(child);
        } else if (dynamic_cast<ASTStringList*>(child)) {
            stringArgs.push_back(child);
        } else if (auto *num = dynamic_cast<ASTNumeric*>(child)) {
            args.numericValue = num->value();
            args.hasNumeric = true;
        }
    }

    // For header/address/envelope tests: first string arg = header names, second = values
    // For exists: all string args = header names
    if (stringArgs.size() >= 2) {
        args.headerNames = _getStrings(stringArgs[0]);
        args.values = _getStrings(stringArgs[1]);
    } else if (stringArgs.size() == 1) {
        args.headerNames = _getStrings(stringArgs[0]);
    }

    return args;
}

std::string ASTSimulationVisitor::_describeTest(ASTNode *node) {
    auto *test = dynamic_cast<ASTTest*>(node);
    if (!test) return "?";

    std::ostringstream ss;
    ss << test->value();

    for (auto *child : test->children()) {
        if (auto *tag = dynamic_cast<ASTTag*>(child)) {
            ss << " " << tag->value();
        } else if (auto *str = dynamic_cast<ASTString*>(child)) {
            ss << " \"" << str->value() << "\"";
        } else if (auto *strList = dynamic_cast<ASTStringList*>(child)) {
            ss << " [";
            bool first = true;
            for (auto *s : strList->children()) {
                if (!first) ss << ", ";
                if (auto *strChild = dynamic_cast<ASTString*>(s)) {
                    ss << "\"" << strChild->value() << "\"";
                }
                first = false;
            }
            ss << "]";
        } else if (auto *num = dynamic_cast<ASTNumeric*>(child)) {
            ss << " " << num->value();
        } else if (auto *subtest = dynamic_cast<ASTTest*>(child)) {
            ss << " " << _describeTest(subtest);
        } else if (auto *tl = dynamic_cast<ASTTestList*>(child)) {
            ss << " (";
            bool first = true;
            for (auto *t : tl->children()) {
                if (!first) ss << ", ";
                ss << _describeTest(t);
                first = false;
            }
            ss << ")";
        }
    }

    return ss.str();
}

bool ASTSimulationVisitor::_matchString(const std::string &value, const std::string &pattern, const std::string &matchType) {
    // Case-insensitive comparison (default comparator is i;ascii-casemap)
    std::string lowerValue = value;
    std::string lowerPattern = pattern;
    std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    std::transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    if (matchType == ":is") {
        return lowerValue == lowerPattern;
    }
    if (matchType == ":contains") {
        return lowerValue.find(lowerPattern) != std::string::npos;
    }
    if (matchType == ":matches") {
        return _globMatch(lowerValue, lowerPattern);
    }
    return false;
}

bool ASTSimulationVisitor::_globMatch(const std::string &str, const std::string &pattern) {
    size_t si = 0, pi = 0;
    size_t starSi = std::string::npos, starPi = std::string::npos;

    while (si < str.size()) {
        if (pi < pattern.size() && pattern[pi] == '*') {
            starPi = pi++;
            starSi = si;
        } else if (pi < pattern.size() && (pattern[pi] == '?' || pattern[pi] == str[si])) {
            pi++;
            si++;
        } else if (starPi != std::string::npos) {
            pi = starPi + 1;
            si = ++starSi;
        } else {
            return false;
        }
    }

    while (pi < pattern.size() && pattern[pi] == '*') {
        pi++;
    }

    return pi == pattern.size();
}

std::string ASTSimulationVisitor::_extractAddressPart(const std::string &headerValue, const std::string &partTag) {
    std::string addr = _extractAddress(headerValue);

    if (partTag == ":localpart") {
        auto at = addr.find('@');
        return (at != std::string::npos) ? addr.substr(0, at) : addr;
    }
    if (partTag == ":domain") {
        auto at = addr.find('@');
        return (at != std::string::npos) ? addr.substr(at + 1) : "";
    }
    return addr; // :all
}

std::string ASTSimulationVisitor::_extractAddress(const std::string &headerValue) {
    // Try to extract address from "Name <addr>" or "<addr>" format
    auto lt = headerValue.find('<');
    auto gt = headerValue.find('>');
    if (lt != std::string::npos && gt != std::string::npos && gt > lt) {
        return headerValue.substr(lt + 1, gt - lt - 1);
    }

    // Trim whitespace and return as-is (bare address)
    std::string trimmed = headerValue;
    auto start = trimmed.find_first_not_of(" \t");
    auto end = trimmed.find_last_not_of(" \t");
    if (start != std::string::npos) {
        return trimmed.substr(start, end - start + 1);
    }
    return trimmed;
}

std::vector<std::string> ASTSimulationVisitor::_getStrings(ASTNode *node) {
    std::vector<std::string> result;
    if (auto *str = dynamic_cast<ASTString*>(node)) {
        result.push_back(std::string(str->value()));
    } else if (auto *strList = dynamic_cast<ASTStringList*>(node)) {
        for (auto *child : strList->children()) {
            if (auto *s = dynamic_cast<ASTString*>(child)) {
                result.push_back(std::string(s->value()));
            }
        }
    }
    return result;
}

// Required visit() implementations (mostly unused -- traversal is via _simulate)
void ASTSimulationVisitor::visit( ASTBlock* ) {}
void ASTSimulationVisitor::visit( ASTBoolean* ) {}
void ASTSimulationVisitor::visit( ASTBranch* ) {}
void ASTSimulationVisitor::visit( ASTCommand* ) {}
void ASTSimulationVisitor::visit( ASTCondition* ) {}
void ASTSimulationVisitor::visit( ASTNoOp* ) {}
void ASTSimulationVisitor::visit( ASTNumeric* ) {}
void ASTSimulationVisitor::visit( ASTRequire* ) {}
void ASTSimulationVisitor::visit( ASTSieve* ) {}
void ASTSimulationVisitor::visit( ASTString* ) {}
void ASTSimulationVisitor::visit( ASTStringList* ) {}
void ASTSimulationVisitor::visit( ASTTag* ) {}
void ASTSimulationVisitor::visit( ASTTest* ) {}
void ASTSimulationVisitor::visit( ASTTestList* ) {}

} // namespace sieve
