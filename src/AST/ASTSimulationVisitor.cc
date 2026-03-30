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

    // If no delivery action was taken, implicit keep (RFC 5228 §4.2)
    if (!_deliveryActionTaken) {
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

            if (name == "keep" || name == "discard" || name == "fileinto" ||
                name == "redirect") {
                _deliveryActionTaken = true;
            }

            std::string actionDesc = name;

            // Extract argument for commands that take one
            if (name == "fileinto" || name == "redirect" || name == "reject" || name == "ereject") {
                for (auto *child : command->children()) {
                    auto *str = dynamic_cast<ASTString*>(child);
                    if (str) {
                        actionDesc += " \"" + _expandVariables(std::string(str->value())) + "\"";
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

        // For other commands (addflag, etc.), just note them
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

        for (const auto &rawHeaderName : args.headerNames) {
            std::string headerName = _expandVariables(rawHeaderName);
            auto values = _email.header(headerName);
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

        for (const auto &rawHeaderName : args.headerNames) {
            std::string headerName = _expandVariables(rawHeaderName);
            auto values = _email.header(headerName);
            for (const auto &hdrVal : values) {
                std::string addr = _extractAddressPart(hdrVal, args.addressPart);
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

        for (const auto &rawEnvPart : args.headerNames) {
            std::string envPart = _expandVariables(rawEnvPart);
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
            } else if (tagVal == ":localpart" || tagVal == ":domain" || tagVal == ":all") {
                args.addressPart = tagVal;
                lastTag.clear();
            } else if (tagVal == ":over" || tagVal == ":under") {
                args.sizeComparator = tagVal;
                lastTag.clear();
            } else if (tagVal == ":raw" || tagVal == ":text" || tagVal == ":content") {
                args.bodyTransform = tagVal;
                lastTag = tagVal;
            } else {
                lastTag.clear();
            }
        } else if (dynamic_cast<ASTString*>(child) || dynamic_cast<ASTStringList*>(child)) {
            if (lastTag == ":content") {
                args.contentTypes = _getStrings(child);
                lastTag.clear();
            } else {
                stringArgs.push_back(child);
                lastTag.clear();
            }
        } else if (auto *num = dynamic_cast<ASTNumeric*>(child)) {
            args.numericValue = num->value();
            args.hasNumeric = true;
            lastTag.clear();
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

std::string ASTSimulationVisitor::_extractAddressPart(const std::string &headerValue, const std::string &partTag) {
    std::string addr = _extractAddress(headerValue);

    if (partTag == ":localpart") {
        auto at = addr.find('@');
        return (at != std::string::npos) ? addr.substr(0, at) : "";
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
