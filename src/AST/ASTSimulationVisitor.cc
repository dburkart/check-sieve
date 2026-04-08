#include <algorithm>
#include <cstdlib>
#include <cstring>
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
    // RFC 5183: fixed simulation defaults (RFC 2606 / RFC 5737 reserved values)
    _environmentItems["domain"]      = "example.com";
    _environmentItems["host"]        = "mail.example.com";
    _environmentItems["location"]    = "MTA";
    _environmentItems["name"]        = "check-sieve";
    _environmentItems["phase"]       = "during";
    _environmentItems["remote-host"] = "remote.example.net";
    _environmentItems["remote-ip"]   = "192.0.2.1";
    _environmentItems["version"]     = "1.0";
}

void ASTSimulationVisitor::walk( ASTSieve *root ) {
    std::cout << "Simulating \"" << _sieveFile << "\" with \"" << _emailFile << "\"" << std::endl;
    std::cout << "---" << std::endl;

    for (auto *child : root->children()) {
        if (_stopped) break;
        _simulate(child);
    }

    // If no delivery action was taken, implicit keep (RFC 5228 §4.2)
    if (!_deliveryActionTaken) {
        std::string keepAction = "keep (implicit)";
        if (!_internalFlags.empty()) {
            keepAction += " :flags \"" + _internalFlags + "\"";
        }
        std::cout << "ACTION: " << keepAction << std::endl;
        _actions.push_back(keepAction);
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
                // Reset match variables before each top-level test (RFC 5229 §3.2)
                _matchVars.assign(10, "");
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

            // RFC 3894: :copy does not cancel implicit keep
            bool hasCopy = false;
            for (auto *child : command->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    if (std::string(tag->value()) == ":copy") { hasCopy = true; break; }
                }
            }

            if (name == "keep" || name == "discard" || name == "fileinto" ||
                name == "redirect") {
                if (!hasCopy) {
                    _deliveryActionTaken = true;
                }
            }

            std::string actionDesc = name;
            std::string lastTag;
            std::string flagsValue;
            bool flagsProvided = false;
            bool positionalArgSet = false;

            for (auto *child : command->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    std::string tagVal = std::string(tag->value());
                    if (tagVal == ":flags") {
                        lastTag = ":flags";
                        flagsProvided = true;
                    } else if (tagVal == ":copy") {
                        actionDesc += " :copy";
                        lastTag.clear();
                    } else {
                        lastTag = tagVal;
                    }
                } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
                    if (lastTag == ":flags") {
                        std::vector<std::string> rawFlagStrs = _getStrings(child);
                        for (auto &f : rawFlagStrs) f = _expandVariables(f);
                        auto flagList = _parseFlags(rawFlagStrs);
                        flagsValue.clear();
                        for (size_t i = 0; i < flagList.size(); ++i) {
                            if (i > 0) flagsValue += " ";
                            flagsValue += flagList[i];
                        }
                        lastTag.clear();
                    } else if (!positionalArgSet &&
                               (name == "fileinto" || name == "redirect" ||
                                name == "reject" || name == "ereject")) {
                        if (auto *str = dynamic_cast<ASTString*>(child)) {
                            actionDesc += " \"" + _expandVariables(std::string(str->value())) + "\"";
                            positionalArgSet = true;
                        }
                        lastTag.clear();
                    } else {
                        lastTag.clear();
                    }
                }
            }

            // RFC 5232: apply :flags for keep and fileinto
            if (name == "keep" || name == "fileinto") {
                if (!flagsProvided) {
                    flagsValue = _internalFlags;
                }
                if (!flagsValue.empty()) {
                    actionDesc += " :flags \"" + flagsValue + "\"";
                }
            }

            std::cout << "ACTION: " << actionDesc << std::endl;
            _actions.push_back(actionDesc);

            if (name == "stop") {
                _stopped = true;
            }
            return;
        }

        // RFC 5229: set command — store variable with optional modifiers
        if (name == "set") {
            std::vector<std::string> modifiers;
            std::vector<std::string> strings;
            for (auto *child : command->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    modifiers.push_back(std::string(tag->value()));
                } else if (auto *str = dynamic_cast<ASTString*>(child)) {
                    strings.push_back(std::string(str->value()));
                }
            }
            if (strings.size() >= 2) {
                std::string varName = strings[strings.size() - 2];
                std::string varValue = strings[strings.size() - 1];
                // Normalize name to lowercase (RFC 5229 §3: case-insensitive)
                std::string lowerName = varName;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                // Expand then apply modifiers
                varValue = _expandVariables(varValue);
                varValue = _applyModifiers(varValue, modifiers);
                _variables[lowerName] = varValue;
                std::cout << "ACTION: set \"" << varName << "\" \"" << varValue << "\"" << std::endl;
                _actions.push_back("set \"" + varName + "\"");
            }
            return;
        }

        // RFC 5230: vacation — out-of-office reply, NOT a delivery action
        if (name == "vacation") {
            std::string lastTag;
            std::string daysValue;
            std::string secondsValue;
            std::string subject;
            std::string from;
            std::string handle;
            std::vector<std::string> addresses;
            bool mime = false;
            std::string reason;

            for (auto *child : command->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    std::string tagVal = std::string(tag->value());
                    if (tagVal == ":mime") {
                        mime = true;
                        lastTag.clear();
                    } else {
                        lastTag = tagVal;
                    }
                } else if (auto *num = dynamic_cast<ASTNumeric*>(child)) {
                    if (lastTag == ":days") {
                        daysValue = std::to_string(num->value());
                    } else if (lastTag == ":seconds") {
                        secondsValue = std::to_string(num->value());
                    }
                    lastTag.clear();
                } else if (auto *strList = dynamic_cast<ASTStringList*>(child)) {
                    if (lastTag == ":addresses") {
                        addresses = _getStrings(strList);
                    }
                    lastTag.clear();
                } else if (auto *str = dynamic_cast<ASTString*>(child)) {
                    std::string sval = _expandVariables(std::string(str->value()));
                    if (lastTag == ":subject") {
                        subject = sval;
                    } else if (lastTag == ":from") {
                        from = sval;
                    } else if (lastTag == ":handle") {
                        handle = sval;
                    } else if (lastTag == ":addresses") {
                        // Single-element list parsed as bare string by parser
                        addresses.push_back(sval);
                    } else {
                        // Positional argument: the reason text
                        reason = sval;
                    }
                    lastTag.clear();
                }
            }

            // Emit tags in canonical order, then reason
            std::ostringstream actionLine;
            actionLine << "vacation";
            if (!daysValue.empty())
                actionLine << " :days " << daysValue;
            if (!secondsValue.empty())
                actionLine << " :seconds " << secondsValue;
            if (!subject.empty())
                actionLine << " :subject \"" << subject << "\"";
            if (!from.empty())
                actionLine << " :from \"" << from << "\"";
            if (!addresses.empty()) {
                actionLine << " :addresses [";
                for (size_t i = 0; i < addresses.size(); ++i) {
                    if (i > 0) actionLine << ", ";
                    actionLine << "\"" << addresses[i] << "\"";
                }
                actionLine << "]";
            }
            if (mime)
                actionLine << " :mime";
            if (!handle.empty())
                actionLine << " :handle \"" << handle << "\"";
            actionLine << " \"" << reason << "\"";

            std::string actionStr = actionLine.str();
            std::cout << "ACTION: " << actionStr << std::endl;
            _actions.push_back("vacation \"" + reason + "\"");
            return;
        }

        // RFC 5232: setflag, addflag, removeflag — manipulate the implicit flag variable
        if (name == "setflag" || name == "addflag" || name == "removeflag") {
            std::vector<ASTNode*> strArgs;
            for (auto *child : command->children()) {
                if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
                    strArgs.push_back(child);
                }
            }

            // Optional first arg is variable name (string only, not list per RFC 5232 §4)
            std::string varName;
            std::vector<std::string> rawFlags;
            if (strArgs.size() >= 2) {
                if (auto *str = dynamic_cast<ASTString*>(strArgs[0])) {
                    varName = _expandVariables(std::string(str->value()));
                }
                rawFlags = _getStrings(strArgs[1]);
            } else if (strArgs.size() == 1) {
                rawFlags = _getStrings(strArgs[0]);
            }

            for (auto &f : rawFlags) f = _expandVariables(f);
            auto newFlags = _parseFlags(rawFlags);

            std::string newFlagStr;
            if (name == "setflag") {
                for (size_t i = 0; i < newFlags.size(); ++i) {
                    if (i > 0) newFlagStr += " ";
                    newFlagStr += newFlags[i];
                }
                _setFlagSet(varName, newFlagStr);
            } else if (name == "addflag") {
                std::string existing = _getFlagSet(varName);
                auto existingFlags = _parseFlags({existing});
                for (const auto &f : newFlags) {
                    std::string fLower = f;
                    std::transform(fLower.begin(), fLower.end(), fLower.begin(),
                                   [](unsigned char c) { return std::tolower(c); });
                    bool found = false;
                    for (const auto &e : existingFlags) {
                        std::string eLower = e;
                        std::transform(eLower.begin(), eLower.end(), eLower.begin(),
                                       [](unsigned char c) { return std::tolower(c); });
                        if (eLower == fLower) { found = true; break; }
                    }
                    if (!found) existingFlags.push_back(f);
                }
                for (size_t i = 0; i < existingFlags.size(); ++i) {
                    if (i > 0) newFlagStr += " ";
                    newFlagStr += existingFlags[i];
                }
                _setFlagSet(varName, newFlagStr);
            } else { // removeflag
                std::string existing = _getFlagSet(varName);
                auto existingFlags = _parseFlags({existing});
                std::vector<std::string> toRemoveLower;
                for (const auto &f : newFlags) {
                    std::string lower = f;
                    std::transform(lower.begin(), lower.end(), lower.begin(),
                                   [](unsigned char c) { return std::tolower(c); });
                    toRemoveLower.push_back(lower);
                }
                std::vector<std::string> remaining;
                for (const auto &e : existingFlags) {
                    std::string eLower = e;
                    std::transform(eLower.begin(), eLower.end(), eLower.begin(),
                                   [](unsigned char c) { return std::tolower(c); });
                    bool remove = false;
                    for (const auto &r : toRemoveLower) {
                        if (eLower == r) { remove = true; break; }
                    }
                    if (!remove) remaining.push_back(e);
                }
                for (size_t i = 0; i < remaining.size(); ++i) {
                    if (i > 0) newFlagStr += " ";
                    newFlagStr += remaining[i];
                }
                _setFlagSet(varName, newFlagStr);
            }

            std::string actionDesc = name;
            if (!varName.empty()) {
                actionDesc += " \"" + varName + "\"";
            }
            // Show flag-list argument
            ASTNode *flagArg = strArgs.size() >= 2 ? strArgs[1] : (!strArgs.empty() ? strArgs[0] : nullptr);
            if (flagArg) {
                auto rawList = _getStrings(flagArg);
                if (rawList.size() == 1) {
                    actionDesc += " \"" + rawList[0] + "\"";
                } else if (rawList.size() > 1) {
                    actionDesc += " [";
                    for (size_t i = 0; i < rawList.size(); ++i) {
                        if (i > 0) actionDesc += ", ";
                        actionDesc += "\"" + rawList[i] + "\"";
                    }
                    actionDesc += "]";
                }
            }

            std::cout << "ACTION: " << actionDesc << std::endl;
            _actions.push_back(name);
            return;
        }

        // For other commands, just note them
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

        // RFC 5231: :value — compare each header value against each key relationally
        if (args.matchType == ":value") {
            for (const auto &rawHeaderName : args.headerNames) {
                std::string headerName = _expandVariables(rawHeaderName);
                auto hdrValues = _applyIndex(_email.header(headerName), args.indexField, args.indexLast);
                for (const auto &hdrVal : hdrValues) {
                    for (const auto &rawPattern : args.values) {
                        std::string pattern = _expandVariables(rawPattern);
                        if (_relationalCompare(hdrVal, pattern, args.relationalOp, args.comparator)) {
                            if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        // RFC 5231: :count — count total header field instances, compare against each key
        if (args.matchType == ":count") {
            long count = 0;
            for (const auto &rawHeaderName : args.headerNames) {
                std::string headerName = _expandVariables(rawHeaderName);
                count += static_cast<long>(_applyIndex(_email.header(headerName), args.indexField, args.indexLast).size());
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                if (_relationalCompare(countStr, pattern, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        for (const auto &rawHeaderName : args.headerNames) {
            std::string headerName = _expandVariables(rawHeaderName);
            auto values = _applyIndex(_email.header(headerName), args.indexField, args.indexLast);
            for (const auto &hdrVal : values) {
                for (const auto &rawPattern : args.values) {
                    std::string pattern = _expandVariables(rawPattern);
                    std::vector<std::string> captures;
                    if (_matchString(hdrVal, pattern, args.matchType, &captures)) {
                        if (args.matchType == ":matches") _matchVars = captures;
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

        // RFC 5231: :value — compare each address against each key relationally
        if (args.matchType == ":value") {
            for (const auto &rawHeaderName : args.headerNames) {
                std::string headerName = _expandVariables(rawHeaderName);
                auto hdrValues = _applyIndex(_email.header(headerName), args.indexField, args.indexLast);
                for (const auto &hdrVal : hdrValues) {
                    auto addrOpt = _extractAddressPart(hdrVal, args.addressPart);
                    if (!addrOpt) continue;
                    std::string addr = *addrOpt;
                    for (const auto &rawPattern : args.values) {
                        std::string pattern = _expandVariables(rawPattern);
                        if (_relationalCompare(addr, pattern, args.relationalOp, args.comparator)) {
                            if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        // RFC 5231: :count — count mailboxes across all named address fields
        if (args.matchType == ":count") {
            long count = 0;
            for (const auto &rawHeaderName : args.headerNames) {
                std::string headerName = _expandVariables(rawHeaderName);
                count += static_cast<long>(_applyIndex(_email.header(headerName), args.indexField, args.indexLast).size());
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                if (_relationalCompare(countStr, pattern, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        for (const auto &rawHeaderName : args.headerNames) {
            std::string headerName = _expandVariables(rawHeaderName);
            auto values = _applyIndex(_email.header(headerName), args.indexField, args.indexLast);
            for (const auto &hdrVal : values) {
                auto addrOpt = _extractAddressPart(hdrVal, args.addressPart);
                if (!addrOpt) continue;
                std::string addr = *addrOpt;
                for (const auto &rawPattern : args.values) {
                    std::string pattern = _expandVariables(rawPattern);
                    std::vector<std::string> captures;
                    if (_matchString(addr, pattern, args.matchType, &captures)) {
                        if (args.matchType == ":matches") _matchVars = captures;
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

        // Helper to map envelope part name to header name
        auto envToHeader = [](const std::string &lowerPart) -> std::string {
            if (lowerPart == "from") return "from";
            if (lowerPart == "to") return "to";
            return "";
        };

        // RFC 5231: :value — compare each envelope address against each key relationally
        if (args.matchType == ":value") {
            for (const auto &rawEnvPart : args.headerNames) {
                std::string envPart = _expandVariables(rawEnvPart);
                std::string lowerPart = envPart;
                std::transform(lowerPart.begin(), lowerPart.end(), lowerPart.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                std::string headerName = envToHeader(lowerPart);
                if (headerName.empty()) continue;
                for (const auto &hdrVal : _email.header(headerName)) {
                    auto addrOpt = _extractAddressPart(hdrVal, args.addressPart);
                    if (!addrOpt) continue;
                    std::string addr = *addrOpt;
                    for (const auto &rawPattern : args.values) {
                        std::string pattern = _expandVariables(rawPattern);
                        if (_relationalCompare(addr, pattern, args.relationalOp, args.comparator)) {
                            if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        // RFC 5231: :count — count envelope addresses
        if (args.matchType == ":count") {
            long count = 0;
            for (const auto &rawEnvPart : args.headerNames) {
                std::string envPart = _expandVariables(rawEnvPart);
                std::string lowerPart = envPart;
                std::transform(lowerPart.begin(), lowerPart.end(), lowerPart.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                std::string headerName = envToHeader(lowerPart);
                if (headerName.empty()) continue;
                count += static_cast<long>(_email.header(headerName).size());
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                if (_relationalCompare(countStr, pattern, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        for (const auto &rawEnvPart : args.headerNames) {
            std::string envPart = _expandVariables(rawEnvPart);
            std::string lowerPart = envPart;
            std::transform(lowerPart.begin(), lowerPart.end(), lowerPart.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            // Map envelope parts to headers
            std::string headerName = envToHeader(lowerPart);
            if (headerName.empty()) continue;

            auto hdrValues = _email.header(headerName);
            for (const auto &hdrVal : hdrValues) {
                auto addrOpt = _extractAddressPart(hdrVal, args.addressPart);
                if (!addrOpt) continue;
                std::string addr = *addrOpt;
                for (const auto &rawPattern : args.values) {
                    std::string pattern = _expandVariables(rawPattern);
                    std::vector<std::string> captures;
                    if (_matchString(addr, pattern, args.matchType, &captures)) {
                        if (args.matchType == ":matches") _matchVars = captures;
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
        for (const auto &rawHeaderName : args.headerNames) {
            std::string headerName = _expandVariables(rawHeaderName);
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
            result = static_cast<long>(_email.size()) > args.numericValue;
        } else if (args.sizeComparator == ":under") {
            result = static_cast<long>(_email.size()) < args.numericValue;
        }

        if (result && !quiet) {
            std::cout << "MATCH: " << _describeTest(test) << std::endl;
        }
        return result;
    }

    if (testName == "body") {
        TestArgs args = _collectTestArgs(test);
        // When no body-transform tag was present, _collectTestArgs routes the single
        // string arg to headerNames (the generic fallback). Fix that up here.
        if (args.values.empty() && !args.headerNames.empty()) {
            args.values = std::move(args.headerNames);
        }
        std::vector<std::string> candidates;

        if (args.bodyTransform == ":raw") {
            // RFC 5173 §5: entire undecoded body as single string
            candidates.push_back(_email.body());
        } else if (args.bodyTransform == ":content") {
            // RFC 5173 §5: match MIME parts with matching content-type
            for (const auto &part : _email.mimeParts()) {
                if (_contentTypeMatches(part.contentType, args.contentTypes)) {
                    candidates.push_back(part.body);
                }
            }
        } else {
            // :text (explicit or default): best-effort — treat as :content "text"
            std::vector<std::string> textType = {"text"};
            for (const auto &part : _email.mimeParts()) {
                if (!part.isMultipart && _contentTypeMatches(part.contentType, textType)) {
                    candidates.push_back(part.body);
                }
            }
        }

        for (const auto &candidate : candidates) {
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                std::vector<std::string> captures;
                if (_matchString(candidate, pattern, args.matchType, &captures)) {
                    if (args.matchType == ":matches") _matchVars = captures;
                    if (!quiet) {
                        std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    // RFC 5229 §5: string test — like header but source comes from the script
    if (testName == "string") {
        TestArgs args = _collectTestArgs(test);
        // _collectTestArgs puts first string arg in headerNames (source-list) and
        // second in values (key-list), which is exactly what we want here.
        for (const auto &rawSource : args.headerNames) {
            std::string source = _expandVariables(rawSource);
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                std::vector<std::string> captures;
                if (_matchString(source, pattern, args.matchType, &captures)) {
                    if (args.matchType == ":matches") _matchVars = captures;
                    if (!quiet) {
                        std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    // RFC 5232: hasflag — test whether a flag variable contains matching flags
    if (testName == "hasflag") {
        std::string matchType = ":is";
        std::string relationalOp;
        std::string comparator;
        std::vector<ASTNode*> strArgs;
        std::string lastTag;

        for (auto *child : test->children()) {
            if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                std::string tagVal = std::string(tag->value());
                if (tagVal == ":is" || tagVal == ":contains" || tagVal == ":matches") {
                    matchType = tagVal;
                    lastTag.clear();
                } else if (tagVal == ":value" || tagVal == ":count") {
                    matchType = tagVal;
                    lastTag = tagVal;
                } else if (tagVal == ":comparator") {
                    lastTag = ":comparator";
                } else {
                    lastTag.clear();
                }
            } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
                if (lastTag == ":value" || lastTag == ":count") {
                    auto strs = _getStrings(child);
                    if (!strs.empty()) relationalOp = strs[0];
                    lastTag.clear();
                } else if (lastTag == ":comparator") {
                    auto strs = _getStrings(child);
                    if (!strs.empty()) comparator = strs[0];
                    lastTag.clear();
                } else {
                    strArgs.push_back(child);
                    lastTag.clear();
                }
            }
        }

        // First string arg = variable-list (optional); last = flag-list (required)
        std::vector<std::string> varList;
        std::vector<std::string> flagPatterns;
        if (strArgs.size() >= 2) {
            varList = _getStrings(strArgs[0]);
            flagPatterns = _getStrings(strArgs[1]);
        } else if (strArgs.size() == 1) {
            varList = {""};  // empty = internal variable
            flagPatterns = _getStrings(strArgs[0]);
        } else {
            return false;
        }

        if (matchType == ":count") {
            long count = 0;
            for (const auto &rawVarName : varList) {
                std::string varName = _expandVariables(rawVarName);
                std::string flagSet = _getFlagSet(varName);
                count += static_cast<long>(_parseFlags({flagSet}).size());
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawPattern : flagPatterns) {
                std::string pattern = _expandVariables(rawPattern);
                if (_relationalCompare(countStr, pattern, relationalOp, comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        for (const auto &rawVarName : varList) {
            std::string varName = _expandVariables(rawVarName);
            std::string flagSet = _getFlagSet(varName);
            auto flags = _parseFlags({flagSet});
            for (const auto &flag : flags) {
                for (const auto &rawPattern : flagPatterns) {
                    std::string pattern = _expandVariables(rawPattern);
                    std::vector<std::string> captures;
                    if (_matchString(flag, pattern, matchType, &captures)) {
                        if (matchType == ":matches") _matchVars = captures;
                        if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // RFC 5183: environment — test named items about the server environment
    if (testName == "environment") {
        TestArgs args = _collectTestArgs(test);

        auto lookupItem = [&](const std::string &raw) -> std::string {
            std::string name = _expandVariables(raw);
            std::transform(name.begin(), name.end(), name.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            return name;
        };

        // RFC 5231: :count — each known item counts as 1, unknown as 0
        if (args.matchType == ":count") {
            long count = 0;
            for (const auto &rawItemName : args.headerNames) {
                std::string lowerItem = lookupItem(rawItemName);
                if (_environmentItems.count(lowerItem)) count += 1;
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                if (_relationalCompare(countStr, pattern, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        // RFC 5231: :value — relational comparison on item's string value
        if (args.matchType == ":value") {
            for (const auto &rawItemName : args.headerNames) {
                std::string lowerItem = lookupItem(rawItemName);
                auto it = _environmentItems.find(lowerItem);
                if (it == _environmentItems.end()) continue;
                for (const auto &rawPattern : args.values) {
                    std::string pattern = _expandVariables(rawPattern);
                    if (_relationalCompare(it->second, pattern, args.relationalOp, args.comparator)) {
                        if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        return true;
                    }
                }
            }
            return false;
        }

        // Standard match types: :is (default), :contains, :matches
        for (const auto &rawItemName : args.headerNames) {
            std::string lowerItem = lookupItem(rawItemName);
            auto it = _environmentItems.find(lowerItem);
            if (it == _environmentItems.end()) continue;  // unknown item → test fails
            for (const auto &rawPattern : args.values) {
                std::string pattern = _expandVariables(rawPattern);
                std::vector<std::string> captures;
                if (_matchString(it->second, pattern, args.matchType, &captures)) {
                    if (args.matchType == ":matches") _matchVars = captures;
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
        }
        return false;
    }

    if (testName == "date") {
        TestArgs args = _collectTestArgs(test);

        // Collect the three positional string args: header-name, date-part, key-list
        // Must skip strings consumed by tags like :value "ge", :comparator "...", :zone "..."
        std::vector<ASTNode*> posNodes;
        {
            std::string lastTag;
            for (auto *child : test->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    std::string tv = tag->value();
                    if (tv == ":value" || tv == ":count" || tv == ":comparator" || tv == ":zone")
                        lastTag = tv;
                    else
                        lastTag.clear();
                } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
                    if (!lastTag.empty())
                        lastTag.clear(); // this string was consumed by preceding tag
                    else
                        posNodes.push_back(child);
                } else if (dynamic_cast<ASTNumeric*>(child)) {
                    lastTag.clear();
                }
            }
        }
        if (posNodes.size() < 3) return false;

        std::string headerName = _expandVariables(_getStrings(posNodes[0])[0]);
        std::string datePart;
        { auto v = _getStrings(posNodes[1]); if (!v.empty()) datePart = v[0]; }
        std::vector<std::string> keyList = _getStrings(posNodes[2]);

        auto headerValues = _applyIndex(_email.header(headerName), args.indexField, args.indexLast);

        if (args.matchType == ":count") {
            long count = 0;
            for (const auto &hdrVal : headerValues) {
                struct tm utcTm; int offsetMin = 0;
                if (_parseRFC2822Date(hdrVal, utcTm, offsetMin)) count = 1;
            }
            std::string countStr = std::to_string(count);
            for (const auto &rawKey : keyList) {
                std::string key = _expandVariables(rawKey);
                if (_relationalCompare(countStr, key, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        for (const auto &hdrVal : headerValues) {
            struct tm utcTm; int headerOffsetMin = 0;
            if (!_parseRFC2822Date(hdrVal, utcTm, headerOffsetMin)) continue;

            int effectiveOffset = 0;
            if (args.zone == "original") {
                effectiveOffset = headerOffsetMin;
            } else if (!args.zone.empty()) {
                int sign = (args.zone[0] == '-') ? -1 : 1;
                std::string digits = args.zone.substr(1);
                if (digits.size() == 4 && std::isdigit((unsigned char)digits[0])) {
                    int hh = std::stoi(digits.substr(0, 2));
                    int mm = std::stoi(digits.substr(2, 2));
                    effectiveOffset = sign * (hh * 60 + mm);
                }
            }

            std::string partValue = _extractDatePart(utcTm, datePart, effectiveOffset);
            if (partValue.empty()) continue;

            if (args.matchType == ":value") {
                for (const auto &rawKey : keyList) {
                    std::string key = _expandVariables(rawKey);
                    if (_relationalCompare(partValue, key, args.relationalOp, args.comparator)) {
                        if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        return true;
                    }
                }
            } else {
                for (const auto &rawKey : keyList) {
                    std::string key = _expandVariables(rawKey);
                    std::vector<std::string> captures;
                    if (_matchString(partValue, key, args.matchType, &captures)) {
                        if (args.matchType == ":matches") _matchVars = captures;
                        if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    if (testName == "currentdate") {
        // Capture script execution time once (RFC 5260 §5)
        if (_scriptTime == 0) _scriptTime = std::time(nullptr);

        TestArgs args = _collectTestArgs(test);

        // Collect two positional string args: date-part, key-list
        // Must skip strings consumed by tags like :value "ge", :comparator "...", :zone "..."
        std::vector<ASTNode*> posNodes;
        {
            std::string lastTag;
            for (auto *child : test->children()) {
                if (auto *tag = dynamic_cast<ASTTag*>(child)) {
                    std::string tv = tag->value();
                    if (tv == ":value" || tv == ":count" || tv == ":comparator" || tv == ":zone")
                        lastTag = tv;
                    else
                        lastTag.clear();
                } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
                    if (!lastTag.empty())
                        lastTag.clear();
                    else
                        posNodes.push_back(child);
                } else if (dynamic_cast<ASTNumeric*>(child)) {
                    lastTag.clear();
                }
            }
        }
        if (posNodes.size() < 2) return false;

        std::string datePart;
        { auto v = _getStrings(posNodes[0]); if (!v.empty()) datePart = v[0]; }
        std::vector<std::string> keyList = _getStrings(posNodes[1]);

        struct tm utcTm;
        gmtime_r(&_scriptTime, &utcTm);

        // :count always returns 1 for currentdate (RFC 5260 §4)
        if (args.matchType == ":count") {
            std::string countStr = "1";
            for (const auto &rawKey : keyList) {
                std::string key = _expandVariables(rawKey);
                if (_relationalCompare(countStr, key, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
            return false;
        }

        int effectiveOffset = 0;
        if (!args.zone.empty() && args.zone != "original") {
            int sign = (args.zone[0] == '-') ? -1 : 1;
            std::string digits = args.zone.substr(1);
            if (digits.size() == 4 && std::isdigit((unsigned char)digits[0])) {
                int hh = std::stoi(digits.substr(0, 2));
                int mm = std::stoi(digits.substr(2, 2));
                effectiveOffset = sign * (hh * 60 + mm);
            }
        }

        std::string partValue = _extractDatePart(utcTm, datePart, effectiveOffset);
        if (partValue.empty()) return false;

        if (args.matchType == ":value") {
            for (const auto &rawKey : keyList) {
                std::string key = _expandVariables(rawKey);
                if (_relationalCompare(partValue, key, args.relationalOp, args.comparator)) {
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
        } else {
            for (const auto &rawKey : keyList) {
                std::string key = _expandVariables(rawKey);
                std::vector<std::string> captures;
                if (_matchString(partValue, key, args.matchType, &captures)) {
                    if (args.matchType == ":matches") _matchVars = captures;
                    if (!quiet) std::cout << "MATCH: " << _describeTest(test) << std::endl;
                    return true;
                }
            }
        }
        return false;
    }

    // Unsupported test
    std::cerr << "WARNING: Unsupported test \"" << testName << "\" -- assuming false" << std::endl;
    return false;
}

ASTSimulationVisitor::TestArgs ASTSimulationVisitor::_collectTestArgs(ASTNode *node) {
    TestArgs args;
    std::vector<ASTNode*> stringArgs;
    std::string lastTag;

    for (auto *child : node->children()) {
        if (auto *tag = dynamic_cast<ASTTag*>(child)) {
            std::string tagVal = tag->value();
            if (tagVal == ":is" || tagVal == ":contains" || tagVal == ":matches") {
                args.matchType = tagVal;
                lastTag.clear();
            } else if (tagVal == ":value" || tagVal == ":count") {
                // RFC 5231: relational match types; next string arg is the operator ("gt", etc.)
                args.matchType = tagVal;
                lastTag = tagVal;
            } else if (tagVal == ":comparator") {
                // RFC 5231: next string arg is the comparator name
                lastTag = ":comparator";
            } else if (tagVal == ":localpart" || tagVal == ":domain" || tagVal == ":all"
                       || tagVal == ":user" || tagVal == ":detail") {
                args.addressPart = tagVal;
                lastTag.clear();
            } else if (tagVal == ":over" || tagVal == ":under") {
                args.sizeComparator = tagVal;
                lastTag.clear();
            } else if (tagVal == ":raw" || tagVal == ":text" || tagVal == ":content") {
                args.bodyTransform = tagVal;
                lastTag = tagVal;
            } else if (tagVal == ":zone") {
                lastTag = ":zone";
            } else if (tagVal == ":originalzone") {
                args.zone = "original";
                lastTag.clear();
            } else if (tagVal == ":index") {
                lastTag = ":index";
            } else if (tagVal == ":last") {
                args.indexLast = true;
                lastTag.clear();
            } else {
                lastTag.clear();
            }
        } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
            if (lastTag == ":value" || lastTag == ":count") {
                // The string immediately following :value/:count is the relational operator
                auto strs = _getStrings(child);
                if (!strs.empty()) args.relationalOp = strs[0];
                lastTag.clear();
            } else if (lastTag == ":comparator") {
                auto strs = _getStrings(child);
                if (!strs.empty()) args.comparator = strs[0];
                lastTag.clear();
            } else if (lastTag == ":content") {
                args.contentTypes = _getStrings(child);
                lastTag.clear();
            } else if (lastTag == ":zone") {
                auto strs = _getStrings(child);
                if (!strs.empty()) args.zone = strs[0];
                lastTag.clear();
            } else {
                stringArgs.push_back(child);
                lastTag.clear();
            }
        } else if (auto *num = dynamic_cast<ASTNumeric*>(child)) {
            if (lastTag == ":index") {
                args.indexField = static_cast<int>(num->value());
                lastTag.clear();
            } else {
                args.numericValue = num->value();
                args.hasNumeric = true;
                lastTag.clear();
            }
        }
    }

    // For body tests: only one string arg (the key-list) → goes to values
    // For header/address/envelope tests: first string arg = header names, second = values
    // For exists: all string args = header names
    bool isBodyTest = !args.contentTypes.empty() || !args.bodyTransform.empty();
    if (isBodyTest) {
        // Body test: the single remaining string arg is the key-list
        if (!stringArgs.empty()) {
            args.values = _getStrings(stringArgs[0]);
        }
    } else if (stringArgs.size() >= 2) {
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

bool ASTSimulationVisitor::_contentTypeMatches(const std::string &partType,
                                                const std::vector<std::string> &patterns) {
    // Normalize partType to lowercase
    std::string lowerPart = partType;
    std::transform(lowerPart.begin(), lowerPart.end(), lowerPart.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    for (const auto &pat : patterns) {
        std::string p = pat;
        std::transform(p.begin(), p.end(), p.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (p.empty()) {
            // Empty string matches all MIME types
            return true;
        }

        auto slashPos = p.find('/');
        if (slashPos == std::string::npos) {
            // Type-only: matches any subtype (e.g. "text" matches "text/plain")
            if (lowerPart.size() > p.size() + 1 &&
                lowerPart.substr(0, p.size()) == p &&
                lowerPart[p.size()] == '/') {
                return true;
            }
        } else {
            // Check for invalid patterns: leading slash, trailing slash, multiple slashes
            bool invalid = (slashPos == 0) ||
                           (slashPos == p.size() - 1) ||
                           (p.find('/', slashPos + 1) != std::string::npos);
            if (!invalid && lowerPart == p) {
                return true;
            }
        }
    }
    return false;
}

bool ASTSimulationVisitor::_relationalCompare(const std::string &left, const std::string &right,
                                               const std::string &op, const std::string &comparator) {
    int cmp = 0;

    if (comparator == "i;ascii-numeric") {
        // RFC 4790: strings of decimal digits compared as unsigned integers.
        // Non-digit strings are less than any digit string; two non-digit strings are equal.
        auto isNumeric = [](const std::string &s) {
            return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
        };
        bool lNum = isNumeric(left);
        bool rNum = isNumeric(right);
        if (!lNum && !rNum) {
            cmp = 0;
        } else if (!lNum) {
            cmp = -1;  // non-numeric < numeric
        } else if (!rNum) {
            cmp = 1;   // numeric > non-numeric
        } else {
            // Both numeric: compare as unsigned long long to handle large values
            unsigned long long lVal = std::stoull(left);
            unsigned long long rVal = std::stoull(right);
            cmp = (lVal < rVal) ? -1 : (lVal > rVal) ? 1 : 0;
        }
    } else if (comparator == "i;octet") {
        // Case-sensitive byte comparison
        cmp = left.compare(right);
    } else {
        // i;ascii-casemap (default) and anything else: case-insensitive string comparison
        std::string lLower = left, rLower = right;
        std::transform(lLower.begin(), lLower.end(), lLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        std::transform(rLower.begin(), rLower.end(), rLower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        cmp = lLower.compare(rLower);
    }

    if (op == "gt") return cmp > 0;
    if (op == "lt") return cmp < 0;
    if (op == "ge") return cmp >= 0;
    if (op == "le") return cmp <= 0;
    if (op == "eq") return cmp == 0;
    if (op == "ne") return cmp != 0;
    return false;  // unknown operator
}

bool ASTSimulationVisitor::_matchString(const std::string &value, const std::string &pattern, const std::string &matchType,
                                         std::vector<std::string> *captures) {
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
        return _globMatch(lowerValue, lowerPattern, captures);
    }
    return false;
}

bool ASTSimulationVisitor::_globMatch(const std::string &str, const std::string &pattern,
                                       std::vector<std::string> *captures) {
    // Tokenize pattern to handle RFC 5228 §2.7.3 backslash escapes:
    // \* → literal '*', \? → literal '?', \X → literal X
    struct Token { enum Kind { LITERAL, QUESTION, STAR } kind; char ch; };
    std::vector<Token> tokens;
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '\\' && i + 1 < pattern.size()) {
            tokens.push_back({Token::LITERAL, pattern[++i]});
        } else if (pattern[i] == '*') {
            tokens.push_back({Token::STAR, 0});
        } else if (pattern[i] == '?') {
            tokens.push_back({Token::QUESTION, 0});
        } else {
            tokens.push_back({Token::LITERAL, pattern[i]});
        }
    }

    // Backtrack matching on token vector.
    // We also track per-STAR the start position in str so we can record captures.
    // Each STAR gets an index (1-based, up to 9) corresponding to RFC 5229 ${1}–${9}.
    struct StarState {
        size_t ti;      // token index of this STAR
        size_t startSi; // where in str this star's match started (minimum position)
    };

    size_t si = 0, ti = 0;
    size_t starTi = std::string::npos, starSi = std::string::npos;

    while (si < str.size()) {
        if (ti < tokens.size() && tokens[ti].kind == Token::STAR) {
            starTi = ti++;
            starSi = si;
        } else if (ti < tokens.size() &&
                   (tokens[ti].kind == Token::QUESTION ||
                    (tokens[ti].kind == Token::LITERAL && tokens[ti].ch == str[si]))) {
            ++ti;
            ++si;
        } else if (starTi != std::string::npos) {
            ti = starTi + 1;
            si = ++starSi;
        } else {
            return false;
        }
    }

    while (ti < tokens.size() && tokens[ti].kind == Token::STAR) {
        ++ti;
    }

    if (ti != tokens.size()) return false;

    // Match succeeded. Populate captures if requested (RFC 5229 §3.2).
    // Walk through token sequence tracking position in str; for each STAR find the
    // shortest span (leftmost end) that still allows the rest of the pattern to match.
    if (captures) {
        captures->assign(10, "");
        (*captures)[0] = str;  // ${0} = full matched string

        int captureIdx = 1;
        size_t si2 = 0;
        for (size_t tIdx = 0; tIdx < tokens.size() && captureIdx <= 9; ++tIdx) {
            if (tokens[tIdx].kind == Token::LITERAL || tokens[tIdx].kind == Token::QUESTION) {
                ++si2; // advance past the character this token consumed
            } else {
                // STAR: find shortest (leftmost end) span from si2.
                // Scan forward from captureStart to find the earliest position where
                // the suffix tokens[nextTi..] can begin to match.
                size_t captureStart = si2;
                size_t nextTi = tIdx + 1;
                size_t captureEnd = str.size(); // default: trailing star consumes rest
                if (nextTi < tokens.size()) {
                    for (size_t s = captureStart; s <= str.size(); ++s) {
                        // Try matching tokens from nextTi against str starting at s.
                        // Consume all consecutive literal/question tokens; stop at STAR
                        // or mismatch.
                        size_t ts = nextTi, ss = s;
                        bool matched = true;
                        while (ts < tokens.size() && tokens[ts].kind != Token::STAR) {
                            if (ss >= str.size()) { matched = false; break; }
                            if (tokens[ts].kind == Token::QUESTION ||
                                (tokens[ts].kind == Token::LITERAL && tokens[ts].ch == str[ss])) {
                                ++ts; ++ss;
                            } else { matched = false; break; }
                        }
                        // If we stopped at a STAR or exhausted non-star tokens at
                        // the end of str (with remaining tokens = stars only), it's ok
                        if (matched) {
                            captureEnd = s;
                            si2 = ss; // advance past the literal/question tokens consumed
                            tIdx = ts - 1; // loop will ++tIdx to ts (next token to process)
                            break;
                        }
                    }
                }
                (*captures)[captureIdx++] = str.substr(captureStart, captureEnd - captureStart);
            }
        }
    }

    return true;
}

std::optional<std::string> ASTSimulationVisitor::_extractAddressPart(const std::string &headerValue, const std::string &partTag) {
    std::string addr = _extractAddress(headerValue);

    if (partTag == ":localpart") {
        auto at = addr.find('@');
        return (at != std::string::npos) ? addr.substr(0, at) : std::string{};
    }
    if (partTag == ":domain") {
        auto at = addr.find('@');
        return (at != std::string::npos) ? addr.substr(at + 1) : std::string{};
    }
    if (partTag == ":user") {
        auto at = addr.find('@');
        std::string localpart = (at != std::string::npos) ? addr.substr(0, at) : addr;
        auto plus = localpart.find('+');
        return (plus != std::string::npos) ? localpart.substr(0, plus) : localpart;
    }
    if (partTag == ":detail") {
        auto at = addr.find('@');
        std::string localpart = (at != std::string::npos) ? addr.substr(0, at) : addr;
        auto plus = localpart.find('+');
        if (plus == std::string::npos) return std::nullopt;  // no subaddress → no match (RFC 5233)
        return localpart.substr(plus + 1);
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

std::string ASTSimulationVisitor::_expandVariables(const std::string &s) const {
    std::string result;
    size_t i = 0;
    while (i < s.size()) {
        if (s[i] == '$' && i + 1 < s.size() && s[i + 1] == '{') {
            size_t start = i + 2;
            size_t end = s.find('}', start);
            if (end == std::string::npos) {
                // No closing brace — copy literally
                result += s[i];
                ++i;
                continue;
            }
            std::string name = s.substr(start, end - start);
            // Validate name: must not be empty
            if (name.empty()) {
                // Invalid — copy literal ${} unchanged
                result += s.substr(i, end - i + 1);
                i = end + 1;
                continue;
            }
            // Check for illegal characters (RFC 5229 §3: letters, digits, underscore,
            // and dot for namespace vars; digit-only = match variable)
            bool isDigit = (name.size() == 1 && std::isdigit((unsigned char)name[0]));
            bool valid = isDigit;
            if (!valid) {
                valid = true;
                for (char c : name) {
                    if (!std::isalnum((unsigned char)c) && c != '_' && c != '.') {
                        valid = false;
                        break;
                    }
                }
            }
            if (!valid) {
                // Invalid reference — copy literally
                result += s.substr(i, end - i + 1);
                i = end + 1;
                continue;
            }
            if (isDigit) {
                size_t idx = name[0] - '0';
                if (idx < _matchVars.size()) {
                    result += _matchVars[idx];
                }
                // else: out of range → empty string (append nothing)
            } else {
                std::string lowerName = name;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                auto it = _variables.find(lowerName);
                if (it != _variables.end()) {
                    result += it->second;
                }
                // else: unknown variable → empty string
            }
            i = end + 1;
        } else {
            result += s[i];
            ++i;
        }
    }
    return result;
}

std::string ASTSimulationVisitor::_applyModifiers(const std::string &value,
                                                    const std::vector<std::string> &modifiers) const {
    if (modifiers.empty()) return value;

    // Assign precedence values (RFC 5229 §4)
    auto precedence = [](const std::string &mod) -> int {
        if (mod == ":lower" || mod == ":upper") return 40;
        if (mod == ":lowerfirst" || mod == ":upperfirst") return 30;
        if (mod == ":quotewildcard") return 20;
        if (mod == ":length") return 10;
        return 0;
    };

    // Sort modifiers by descending precedence (apply highest first)
    std::vector<std::string> sorted = modifiers;
    std::sort(sorted.begin(), sorted.end(), [&](const std::string &a, const std::string &b) {
        return precedence(a) > precedence(b);
    });

    std::string result = value;
    for (const auto &mod : sorted) {
        if (mod == ":lower") {
            std::transform(result.begin(), result.end(), result.begin(),
                           [](unsigned char c) { return std::tolower(c); });
        } else if (mod == ":upper") {
            std::transform(result.begin(), result.end(), result.begin(),
                           [](unsigned char c) { return std::toupper(c); });
        } else if (mod == ":lowerfirst") {
            if (!result.empty()) result[0] = static_cast<char>(std::tolower((unsigned char)result[0]));
        } else if (mod == ":upperfirst") {
            if (!result.empty()) result[0] = static_cast<char>(std::toupper((unsigned char)result[0]));
        } else if (mod == ":quotewildcard") {
            std::string escaped;
            for (char c : result) {
                if (c == '*' || c == '?' || c == '\\') escaped += '\\';
                escaped += c;
            }
            result = escaped;
        } else if (mod == ":length") {
            result = std::to_string(result.size());
        }
    }
    return result;
}

// RFC 5232: Parse a list of flag strings (may contain space-separated flags within each string)
// Returns deduplicated list (case-insensitive), preserving original case of first occurrence.
std::vector<std::string> ASTSimulationVisitor::_parseFlags(const std::vector<std::string> &flagStrings) {
    std::vector<std::string> result;
    std::vector<std::string> seenLower;
    for (const auto &s : flagStrings) {
        std::istringstream iss(s);
        std::string token;
        while (iss >> token) {
            std::string lower = token;
            std::transform(lower.begin(), lower.end(), lower.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            bool found = false;
            for (const auto &seen : seenLower) {
                if (seen == lower) { found = true; break; }
            }
            if (!found) {
                result.push_back(token);
                seenLower.push_back(lower);
            }
        }
    }
    return result;
}

std::string ASTSimulationVisitor::_getFlagSet(const std::string &varName) const {
    if (varName.empty()) return _internalFlags;
    std::string lower = varName;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    auto it = _variables.find(lower);
    return (it != _variables.end()) ? it->second : "";
}

void ASTSimulationVisitor::_setFlagSet(const std::string &varName, const std::string &flags) {
    if (varName.empty()) {
        _internalFlags = flags;
    } else {
        std::string lower = varName;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        _variables[lower] = flags;
    }
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

// RFC 5260: apply :index / :last selection to a vector of header values
std::vector<std::string> ASTSimulationVisitor::_applyIndex(
    const std::vector<std::string> &values, int indexField, bool indexLast)
{
    if (indexField <= 0 || values.empty()) return values;
    size_t idx;
    if (indexLast) {
        if (static_cast<size_t>(indexField) > values.size()) return {};
        idx = values.size() - static_cast<size_t>(indexField);
    } else {
        idx = static_cast<size_t>(indexField) - 1;
        if (idx >= values.size()) return {};
    }
    return { values[idx] };
}

// RFC 5260: parse an RFC 2822 date-time value into a UTC struct tm.
// Handles plain Date: header values and Received: values (date after last ';').
// offsetMinutes is set to the original UTC offset from the header (e.g. -300 for -0500).
bool ASTSimulationVisitor::_parseRFC2822Date(const std::string &headerValue,
                                              struct tm &tmOut, int &offsetMinutes)
{
    offsetMinutes = 0;

    // Received: headers embed the date after the last ';'
    std::string toParse = headerValue;
    size_t semi = headerValue.rfind(';');
    if (semi != std::string::npos) {
        toParse = headerValue.substr(semi + 1);
        size_t first = toParse.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return false;
        toParse = toParse.substr(first);
    }

    // Manually extract trailing timezone token to avoid strptime %z portability issues
    size_t lastSpace = toParse.rfind(' ');
    if (lastSpace == std::string::npos) return false;
    std::string tzToken = toParse.substr(lastSpace + 1);
    std::string dateStr = toParse.substr(0, lastSpace);

    // Parse numeric timezone: "+0500" / "-0700" (5 chars including sign)
    if (tzToken.size() == 5 && (tzToken[0] == '+' || tzToken[0] == '-')) {
        int sign = (tzToken[0] == '+') ? 1 : -1;
        std::string digits = tzToken.substr(1);
        if (std::isdigit((unsigned char)digits[0])) {
            int hh = std::stoi(digits.substr(0, 2));
            int mm = std::stoi(digits.substr(2, 2));
            offsetMinutes = sign * (hh * 60 + mm);
        }
    }
    // Named zones (UTC, GMT, UT, etc.) remain at 0

    memset(&tmOut, 0, sizeof(tmOut));
    const char *p = strptime(dateStr.c_str(), "%a, %d %b %Y %H:%M:%S", &tmOut);
    if (!p) p = strptime(dateStr.c_str(), "%d %b %Y %H:%M:%S", &tmOut);
    if (!p) return false;

    // tmOut fields are set for the header's local timezone; convert to UTC
    time_t asLocal = timegm(&tmOut);
    if (asLocal == static_cast<time_t>(-1)) return false;
    time_t asUTC = asLocal - (static_cast<time_t>(offsetMinutes) * 60);
    gmtime_r(&asUTC, &tmOut);
    return true;
}

// RFC 5260: extract the named date-part from a UTC struct tm, applying offsetMinutes
// to shift into the effective timezone before formatting.
std::string ASTSimulationVisitor::_extractDatePart(const struct tm &utcTm,
                                                    const std::string &part,
                                                    int offsetMinutes)
{
    // Shift UTC time into the effective timezone
    struct tm shifted = utcTm;
    time_t t = timegm(&shifted);
    t += static_cast<time_t>(offsetMinutes) * 60;
    struct tm local;
    gmtime_r(&t, &local);

    char buf[64];
    if (part == "year") {
        snprintf(buf, sizeof(buf), "%04d", local.tm_year + 1900);
    } else if (part == "month") {
        snprintf(buf, sizeof(buf), "%02d", local.tm_mon + 1);
    } else if (part == "day") {
        snprintf(buf, sizeof(buf), "%02d", local.tm_mday);
    } else if (part == "date") {
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
                 local.tm_year + 1900, local.tm_mon + 1, local.tm_mday);
    } else if (part == "hour") {
        snprintf(buf, sizeof(buf), "%02d", local.tm_hour);
    } else if (part == "minute") {
        snprintf(buf, sizeof(buf), "%02d", local.tm_min);
    } else if (part == "second") {
        snprintf(buf, sizeof(buf), "%02d", local.tm_sec);
    } else if (part == "time") {
        snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
                 local.tm_hour, local.tm_min, local.tm_sec);
    } else if (part == "weekday") {
        snprintf(buf, sizeof(buf), "%d", local.tm_wday);
    } else if (part == "zone") {
        int absOff = std::abs(offsetMinutes);
        snprintf(buf, sizeof(buf), "%c%02d%02d",
                 offsetMinutes >= 0 ? '+' : '-', absOff / 60, absOff % 60);
    } else if (part == "iso8601") {
        if (offsetMinutes == 0) {
            snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                     local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
                     local.tm_hour, local.tm_min, local.tm_sec);
        } else {
            int absOff = std::abs(offsetMinutes);
            snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d%c%02d:%02d",
                     local.tm_year + 1900, local.tm_mon + 1, local.tm_mday,
                     local.tm_hour, local.tm_min, local.tm_sec,
                     offsetMinutes >= 0 ? '+' : '-', absOff / 60, absOff % 60);
        }
    } else if (part == "std11") {
        strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S", &local);
        int absOff = std::abs(offsetMinutes);
        char off[16];
        snprintf(off, sizeof(off), " %c%02d%02d",
                 offsetMinutes >= 0 ? '+' : '-', absOff / 60, absOff % 60);
        return std::string(buf) + off;
    } else if (part == "julian") {
        int Y = local.tm_year + 1900, M = local.tm_mon + 1, D = local.tm_mday;
        int a = (14 - M) / 12, y = Y + 4800 - a, m = M + 12 * a - 3;
        long jdn = D + (153 * m + 2) / 5 + 365L * y + y / 4 - y / 100 + y / 400 - 32045;
        snprintf(buf, sizeof(buf), "%ld", jdn - 2400001L);
    } else {
        return "";  // unknown date-part
    }
    return std::string(buf);
}

} // namespace sieve
