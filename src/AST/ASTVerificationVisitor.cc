#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#include "ASTVerificationVisitor.hh"

namespace sieve
{

ASTVerificationVisitor::ASTVerificationVisitor(struct parse_options options)
    : _options(std::move( options ))
    , _verification_result()
    , _require_lookup()
    , _required_capabilities( nullptr ) {
    _init();
}

void ASTVerificationVisitor::walk( ASTSieve *root ) {
    this->_traverse_tree( root );
}

void ASTVerificationVisitor::_traverse_tree( sieve::ASTNode *node ) {
    node->accept(*this);

    if (result().status)
        return;

    std::vector<sieve::ASTNode *> children = node->children();
    for (auto child : children) {
        _traverse_tree(child);
    }
}

void ASTVerificationVisitor::visit( ASTBlock* node ) {

}

void ASTVerificationVisitor::visit( ASTBoolean* node ) {

}

void ASTVerificationVisitor::visit( ASTBranch* node ) {

}

void ASTVerificationVisitor::visit( ASTCommand* node ) {
    std::string value_lower = node->value();
    std::transform(value_lower.begin(), value_lower.end(), value_lower.begin(), ::tolower);

    if (!_command_map[value_lower]) {
        _verification_result = {1, node->location(), "Unrecognized command \"" + node->value() + "\"."};

        auto require = _require_lookup.find(value_lower);
        if (require != _require_lookup.end()) {
            _verification_result.hint = "\n(Hint: you might need to require \"" + std::string(require->second) + "\")";
        }

        return;
    }

    auto r = _command.validate(node);
    if (!r.result()) {
        auto hint = _command.usage(node);

        if (!r.hint().empty()) {
            if (r.hint_as_error()) {
                hint = r.hint();
            } else {
                hint += "\n\n(Hint: " + r.hint() + ")";
            }
        }

        auto location = node->location();
        if (r.offset() != 0) {
            location = node->children()[r.offset()]->location();
        }

        _verification_result = {
            1,
            location,
            "Incorrect syntax for command \"" + node->value() + "\".",
            hint,
        };
    }
}

void ASTVerificationVisitor::visit( ASTCondition* node ) {

}

void ASTVerificationVisitor::visit( ASTNoOp* ) {
    // No Op
}

void ASTVerificationVisitor::visit( ASTNumeric* node ) {

}

void ASTVerificationVisitor::visit( ASTRequire* node ) {
    std::vector<sieve::ASTNode *> children = node->children();

    // If we are requiring multiple modules, we need our grandchildren
    if (dynamic_cast<ASTStringList *>(children[0]) != nullptr) {
        _required_capabilities = dynamic_cast<ASTStringList *>(children[0]);
        children = children[0]->children();
    }

    for (const auto & it : children) {
        const auto *child = dynamic_cast<ASTString *>(it);
        _capability_map[child->value()] = true;
        _enable_capability(child->value());
    }
}

void ASTVerificationVisitor::visit( ASTSieve* node ) {

}

void ASTVerificationVisitor::visit( ASTString* node ) {

}

void ASTVerificationVisitor::visit( ASTStringList* node ) {
    if ( _options.string_list_max_length ) {
        if ( node->length() > _options.string_list_max_length ) {
            _verification_result = {1, node->location(), "String list length is " + std::to_string(node->length()) + ", but the configured maximum is " + std::to_string(_options.string_list_max_length)};
        }
    }
}

void ASTVerificationVisitor::visit( ASTTag* node ) {
    std::string value_lower = node->value();
    std::transform(value_lower.begin(), value_lower.end(), value_lower.begin(), ::tolower);

    if (!_tag_map[value_lower]) {
        _verification_result = {1, node->location(), "Unrecognized tag \"" + node->value() + "\"."};

        auto require = _require_lookup.find(value_lower);
        if (require != _require_lookup.end()) {
            _verification_result.hint = "\n(Hint: you might need to require \"" + std::string(require->second) + "\")";
        }

        return;
    }

    auto r =  _tag.validate(node);
    if (!r.result()) {
        auto hint = _tag.usage(node);

        if (!r.hint().empty()) {
            if (r.hint_as_error()) {
                hint = r.hint();
            } else {
                hint += "\n\n(Hint: " + r.hint() + ")";
            }
        }

        auto location = node->location();
        if (r.offset() != 0) {
            location = node->children()[r.offset()]->location();
        }

        _verification_result = {
            1,
            location,
            "Incorrect syntax for tag \"" + node->value() + "\".",
            hint,
        };
    }
}

void ASTVerificationVisitor::visit( ASTTest* node ) {
    std::vector<sieve::ASTNode *> children = node->children();
    std::string value_lower = node->value();
    std::transform(value_lower.begin(), value_lower.end(), value_lower.begin(), ::tolower);
    const ASTTag *first_match_tag = nullptr;
    const ASTTag *second_match_tag = nullptr;

    if (!_test_map[value_lower]) {
        _verification_result = {1, node->location(), "Unrecognized test \"" + node->value() + "\"."};

        auto require = _require_lookup.find(value_lower);
        if (require != _require_lookup.end()) {
            _verification_result.hint = "\n(Hint: you might need to require \"" + std::string(require->second) + "\")";
        }

        return;
    }

    for (auto & it : children) {
        // Are we an ASTTag?
        const ASTTag *child = dynamic_cast<ASTTag*>(it);

        if (child != nullptr) {
            // Ensure that we have only _one_ of :is, :contains, or :matches
            if (child->value() == ":is" || child->value() == ":contains" || child->value() == ":matches") {
                if (first_match_tag == nullptr)
                    first_match_tag = child;
                else if (second_match_tag == nullptr)
                    second_match_tag = child;
            }
        }

        // If it's an "ihave" test, we need to enable any specified capabilities
        if (value_lower == "ihave") {
            const ASTString *capability = dynamic_cast<ASTString*>(it);

            if (capability != nullptr) {
                _enable_capability(capability->value());
            }
        }
    }

    if (second_match_tag != nullptr) {
        _verification_result = {1, second_match_tag->location(), "Only one match type tag is allowed; first match type tag was \"" + first_match_tag->value() + "\"."};
    }


    auto r = _test.validate(node);
    if (!r.result()) {
        auto hint = _test.usage(node);

        if (!r.hint().empty()) {
            if (r.hint_as_error()) {
                hint = r.hint();
            } else {
                hint += "\n\n(Hint: " + r.hint() + ")";
            }
        }

        _verification_result = {
            1,
            node->location(),
            "Incorrect syntax for test \"" + node->value() + "\".",
            hint,
        };
    }
}

void ASTVerificationVisitor::visit( ASTTestList* node ) {
    
}

//-- Private methods
void ASTVerificationVisitor::_init() {
    _command_map["addheader"] = true;
    _command_map["keep"] = true;
    _command_map["deleteheader"] = true;
    _command_map["discard"] = true;
    _command_map["redirect"] = true;
    _command_map["stop"] = true;

    _test_map["allof"] = true;
    _test_map["anyof"] = true;
    _test_map["address"] = true;
    _test_map["envelope"] = true;
    _test_map["header"] = true;
    _test_map["size"] = true;
    _test_map["not"] = true;
    _test_map["exists"] = true;
    _test_map["$command_result"] = true; // Special test indicating the result of a command

    _tag_map[":is"] = true;
    _tag_map[":contains"] = true;
    _tag_map[":matches"] = true;
    _tag_map[":last"] = true;
    _tag_map[":localpart"] = true;
    _tag_map[":domain"] = true;
    _tag_map[":all"] = true;
    _tag_map[":over"] = true;
    _tag_map[":under"] = true;

    // TODO: "Comparators other than "i;octet" and "i;ascii-casemap" must be
    // declared with require, as they are extensions"
    _tag_map[":comparator"] = true;

    // Set up references to this visitor for introspection
    _command.set_visitor(this);
    _test.set_visitor(this);
    _tag.set_visitor(this);

}

void ASTVerificationVisitor::_enable_capability(std::string_view capability) {
    if (!_options.all_supported_capabilities && !_options.capabilities[capability])
    {
        _verification_result = {
                1,
                yy::location(),
                "Capability \"" + std::string{ capability } + "\" was requested, but does not seem to be supported by your mail server."
        };
        return;
    }

    // "copy"
    // RFC 3894
    if (capability == "copy") {
        _tag_map[":copy"] = true;
    }

    // Require hints
    _require_lookup[":copy"] = "copy";

    // "body"
    // RFC 5173
    if (capability == "body") {
        _test_map["body"] = true;
        _tag_map[":raw"] = true;
        _tag_map[":content"] = true;
        _tag_map[":text"] = true;
    }

    // Require hints
    _require_lookup["body"] = "body";
    _require_lookup[":raw"] = "body";
    _require_lookup[":content"] = "body";
    _require_lookup[":text"] = "body";

    // "environment"
    // RFC 5183
    if (capability == "environment") {
        _test_map["environment"] = true;
    }

    // Require hints
    _require_lookup["environment"] = "environment";

    // "fileinto"
    // RFC 5228
    if (capability == "fileinto") {
        _command_map["fileinto"] = true;
    }

    // Require hints
    _require_lookup["fileinto"] = "fileinto";

    // "reject"
    // RFC 5228
    if (capability == "reject") {
        _command_map["reject"] = true;
    }

    // Require hints
    _require_lookup["reject"] = "reject";

    // "variables"
    // RFC 5229
    if (capability == "variables") {
        _command_map["set"] = true;
        _test_map["string"] = true;
        _tag_map[":lower"] = true;
        _tag_map[":upper"] = true;
        _tag_map[":lowerfirst"] = true;
        _tag_map[":upperfirst"] = true;
        _tag_map[":quotewildcard"] = true;
        _tag_map[":length"] = true;
    }

    // Require hints
    _require_lookup["set"] = "variables";
    _require_lookup["string"] = "variables";
    _require_lookup[":lower"] = "variables";
    _require_lookup[":upper"] = "variables";
    _require_lookup[":lowerfirst"] = "variables";
    _require_lookup[":upperfirst"] = "variables";
    _require_lookup[":quotewildcard"] = "variables";
    _require_lookup[":length"] = "variables";
    _require_lookup[":encodeurl"] = "enotify";
    _require_lookup["global"] = "include";
    _require_lookup[":quoteregex"] = "regex";

    // "vacation"
    // RFC 5230
    if (capability == "vacation") {
        _command_map["vacation"] = true;
        _tag_map[":days"] = true;
        _tag_map[":from"] = true;
        _tag_map[":subject"] = true;
        _tag_map[":addresses"] = true;
        _tag_map[":mime"] = true;
        _tag_map[":handle"] = true;
    }

    // Require hints
    _require_lookup["vacation"] = "vacation";
    _require_lookup[":days"] = "vacation";
    _require_lookup[":from"] = "vacation";
    _require_lookup[":subject"] = "vacation";
    _require_lookup[":addresses"] = "vacation";
    _require_lookup[":mime"] = "vacation";
    _require_lookup[":handle"] = "vacation";

    // "relational"
    // RFC 5231
    if (capability == "relational") {
        _tag_map[":count"] = true;
        _tag_map[":value"] = true;
    }

    // Require hints
    _require_lookup[":count"] = "relational";
    _require_lookup[":value"] = "relational";

    // "imap4flags"
    // RFC 5232
    if (capability == "imap4flags") {
        _command_map["setflag"] = true;
        _command_map["addflag"] = true;
        _command_map["removeflag"] = true;
        _test_map["hasflag"] = true;
        _tag_map[":flags"] = true;
    }

    // Require hints
    _require_lookup["setflag"] = "imap4flags";
    _require_lookup["addflag"] = "imap4flags";
    _require_lookup["removeflag"] = "imap4flags";
    _require_lookup["hasflag"] = "imap4flags";
    _require_lookup[":flags"] = "imap4flags";

    // "subaddress"
    // RFC 5233
    if (capability == "subaddress") {
        _tag_map[":user"] = true;
        _tag_map[":detail"] = true;
    }

    // Require hints
    _require_lookup[":user"] = "subaddress";
    _require_lookup[":detail"] = "subaddress";

    // "date"
    // RFC 5260
    if (capability == "date") {
        _test_map["date"] = true;
        _test_map["currentdate"] = true;
        _tag_map[":zone"] = true;
        _tag_map[":originalzone"] = true;
    }

    // Require hints
    _require_lookup["date"] = "date";
    _require_lookup["currentdate"] = "date";
    _require_lookup[":zone"] = "date";
    _require_lookup[":originalzone"] = "date";

    // "index"
    // RFC 5260
    if (capability == "index") {
        _tag_map[":index"] = true;
        _tag_map[":last"] = true;
    }

    // Require hints
    _require_lookup[":index"] = "index";
    _require_lookup[":last"] = "index";

    // "spamtest" or "spamtestplus"
    // RFC 5235
    if (capability == "spamtest" || capability == "spamtestplus") {
        _test_map["spamtest"] = true;
        _tag_map[":percent"] = true;
    }

    // Require hints
    _require_lookup["spamtest"] = "spamtest";
    _require_lookup[":percent"] = "spamtest";

    // "virustest"
    // RFC 5235
    if (capability == "virustest") {
        _test_map["virustest"] = true;
    }

    // Require hints
    _require_lookup["virustest"] = "virustest";

    // "ereject"
    // RFC 5429
    if (capability == "ereject") {
        _command_map["ereject"] = true;
    }

    // Require hints
    _require_lookup["ereject"] = "ereject";

    // "enotify"
    // RFC 5435
    if (capability == "enotify") {
        _command_map["notify"] = true;
        _tag_map[":from"] = true;
        _tag_map[":importance"] = true;
        _tag_map[":options"] = true;
        _tag_map[":message"] = true;
        _test_map["valid_notify_method"] = true;
        _test_map["notify_method_capability"] = true;

        // The :encodeurl tag can only be used if both "enotify" and
        // "variables" are required
        if (has_required("variables")) {
            _tag_map[":encodeurl"] = true;
        }

        _require_lookup[":encodeurl"] = "variables";
    }

    // Require hints
    _require_lookup["notify"] = "enotify";
    _require_lookup[":from"] = "enotify";
    _require_lookup[":importance"] = "enotify";
    _require_lookup[":options"] = "enotify";
    _require_lookup[":message"] = "enotify";
    _require_lookup["valid_notify_method"] = "enotify";
    _require_lookup["notify_method_capability"] = "enotify";

    // "ihave"
    // RFC 5463
    if (capability == "ihave") {
        _command_map["error"] = true;
        _test_map["ihave"] = true;
    }

    // Require hints
    _require_lookup["error"] = "ihave";
    _require_lookup["ihave"] = "ihave";

    // "mailbox"
    // RFC 5490
    if (capability == "mailbox") {
        _test_map["mailboxexists"] = true;
        _tag_map[":create"] = true;
    }

    // Require hints
    _require_lookup["mailboxexists"] = "mailbox";
    _require_lookup[":create"] = "mailbox";

    // "mboxmetadata"
    // RFC 5490
    if (capability == "mboxmetadata") {
        _test_map["metadata"] = true;
        _test_map["metadataexists"] = true;
    }

    // Require hints
    _require_lookup["metadata"] = "mboxmetadata";
    _require_lookup["metadataexists"] = "mboxmetadata";

    // "servermetadata"
    // RFC 5490
    if (capability == "servermetadata") {
        _test_map["servermetadata"] = true;
        _test_map["servermetadataexists"] = true;
    }

    // Require hints
    _require_lookup["servermetadata"] = "servermetadata";
    _require_lookup["servermetadataexists"] = "servermetadata";

    // "foreverypart"
    // RFC 5703
    if (capability == "foreverypart") {
        _command_map["foreverypart"] = true;
        _command_map["break"] = true;
        _tag_map[":name"] = true;
    }

    // Require hints
    _require_lookup["foreverypart"] = "foreverypart";
    _require_lookup["break"] = "foreverypart";
    _require_lookup[":name"] = "foreverypart";

    // "mime"
    // RFC 5703
    if (capability == "mime") {
        _tag_map[":mime"] = true;
        _tag_map[":type"] = true;
        _tag_map[":subtype"] = true;
        _tag_map[":contenttype"] = true;
        _tag_map[":param"] = true;
        _tag_map[":anychild"] = true;
    }

    // Require hints
    _require_lookup[":mime"] = "mime";
    _require_lookup[":type"] = "mime";
    _require_lookup[":subtype"] = "mime";
    _require_lookup[":contenttype"] = "mime";
    _require_lookup[":param"] = "mime";
    _require_lookup[":anychild"] = "mime";

    // "extracttext"
    // RFC 5703
    if (capability == "extracttext") {
        _command_map["extracttext"] = true;
        _tag_map[":first"] = true;
    }

    // Require hints
    _require_lookup["extracttext"] = "extracttext";
    _require_lookup[":first"] = "extracttext";

    // "replace"
    // RFC 5703
    if (capability == "replace") {
        _command_map["replace"] = true;
        _tag_map[":subject"] = true;
        _tag_map[":from"] = true;
    }

    // Require hints
    _require_lookup["replace"] = "replace";
    _require_lookup[":subject"] = "replace";
    _require_lookup[":from"] = "replace";

    // "enclose"
    // RFC 5703
    if (capability == "enclose") {
        _command_map["enclose"] = true;
        _tag_map[":subject"] = true;
        _tag_map[":headers"] = true;
    }

    // Require hints
    _require_lookup["enclose"] = "enclose";
    _require_lookup[":subject"] = "enclose";
    _require_lookup[":headers"] = "enclose";

    // "redirect-dsn"
    // RFC 6009
    if (capability == "redirect-dsn") {
        _tag_map[":notify"] = true;
        _tag_map[":ret"] = true;
    }

    // Require hints
    _require_lookup[":notify"] = "redirect-dsn";
    _require_lookup[":ret"] = "redirect-dsn";

    // "redirect-deliverby"
    // RFC 6009
    if (capability == "redirect-deliverby") {
        _tag_map[":bytimerelative"] = true;
        _tag_map[":bytimeabsolute"] = true;
        _tag_map[":bymode"] = true;
        _tag_map[":bytrace"] = true;
    }

    // Require hints
    _require_lookup[":bytimerelative"] = "redirect-deliverby";
    _require_lookup[":bytimeabsolute"] = "redirect-deliverby";
    _require_lookup[":bymode"] = "redirect-deliverby";
    _require_lookup[":bytrace"] = "redirect-deliverby";

    // "include"
    // RFC 6609
    if (capability == "include") {
        _command_map["include"] = true;
        _command_map["return"] = true;
        _tag_map[":once"] = true;
        _tag_map[":optional"] = true;
        _tag_map[":personal"] = true;
        _tag_map[":global"] = true;

        // The "global" command can only be used if both "include" and
        // "variables" are required
        if (has_required("variables")) {
            _command_map["global"] = true;
        }
        _require_lookup["global"] = "variables";
    }

    // Require hints
    _require_lookup["include"] = "include";
    _require_lookup["return"] = "include";
    _require_lookup[":once"] = "include";
    _require_lookup[":optional"] = "include";
    _require_lookup[":personal"] = "include";
    _require_lookup[":global"] = "include";

    // "convert"
    // RFC 6658
    if (capability == "convert") {
        _command_map["convert"] = true;
        _test_map["convert"] = true;
    }

    // Require hints
    _require_lookup["convert"] = "convert";

    // "extlists"
    // RFC 6134
    if (capability == "extlists") {
        _tag_map[":list"] = true;
        _test_map["valid_ext_list"] = true;
    }

    // Require hints
    _require_lookup[":list"] = "extlists";
    _require_lookup["valid_ext_list"] = "extlists";

    // Proposed Standards

    // "duplicate"
    // RFC 7352
    // https://datatracker.ietf.org/doc/html/rfc7352.html
    if (capability == "duplicate") {
        _test_map["duplicate"] = true;
        _tag_map[":handle"] = true;
        _tag_map[":header"] = true;
        _tag_map[":uniqueid"] = true;
        _tag_map[":seconds"] = true;
    }

    // Require hints
    _require_lookup["duplicate"] = "duplicate";
    _require_lookup[":handle"] = "duplicate";
    _require_lookup[":header"] = "duplicate";
    _require_lookup[":uniqueid"] = "duplicate";
    _require_lookup[":seconds"] = "duplicate";

    // "special-use"
    // RFC 8579
    // https://www.rfc-editor.org/rfc/rfc8579
    if (capability == "special-use") {
        _test_map["specialuse_exists"] = true;
        _tag_map[":specialuse"] = true;
    }

    // Require hints
    _require_lookup["specialuse_exists"] = "special-use";
    _require_lookup[":specialuse"] = "special-use";

    // "fcc"
    // RFC 8580
    // https://www.rfc-editor.org/rfc/rfc8580.html
    if (capability == "fcc") {
        _tag_map[":fcc"] = true;
    }

    // Require hints
    _require_lookup[":fcc"] = "fcc";

    // "mailboxid"
    // RFC 9042
    // https://www.rfc-editor.org/rfc/rfc9042.html
    if (capability == "mailboxid") {
        _test_map["mailboxidexists"] = true;
        _tag_map[":mailboxid"] = true;
    }

    // Hints
    _require_lookup["mailboxidexists"] = "mailboxid";
    _require_lookup[":mailboxid"] = "mailboxid";

    // DRAFT RFCs

    // "regex"
    // (https://tools.ietf.org/html/draft-ietf-sieve-regex-01)
    if (capability == "regex") {
        _tag_map[":regex"] = true;

        // The ":quoteregex" command is supported if both "regex" and
        // "variables" are required
        if (has_required("variables")) {
            _tag_map[":quoteregex"] = true;
        }

        _require_lookup[":quoteregex"] = "variables";
    }

    // Require hints
    _require_lookup[":regex"] = "regex";

    // VENDORED

    // "vnd.proton.expire"
    // (https://proton.me/support/sieve-advanced-custom-filters#managing-expiration)
    if (capability == "vnd.proton.expire") {
        _command_map["expire"] = true;
        _command_map["unexpire"] = true;
        _test_map["hasexpiration"] = true;

        // The "expiration" test is supported if both "vnd.proton.expire" and
        // "comparator-i;ascii-numeric" are required.
        if (has_required("comparator-i;ascii-numeric")) {
            _test_map["expiration"] = true;
        }
    }

    // Require hints
    _require_lookup["expire"] = "vnd.proton.expire";
    _require_lookup["unexpire"] = "vnd.proton.expire";
    _require_lookup["hasexpiration"] = "vnd.proton.expire";
    _require_lookup["expiration"] = "vnd.proton.expire";

    // "vnd.proton.eval"
    // (https://proton.me/support/sieve-advanced-custom-filters#transforming-variables)
    // depends on "variables"
    if (capability == "vnd.proton.eval" && has_required("variables")) {
        _tag_map[":eval"] = true;
    }

    // Require hints
    _require_lookup[":eval"] = "vnd.proton.eval";

    // vnd.dovecot.pipe
    // (https://raw.githubusercontent.com/dovecot/pigeonhole/refs/heads/main/doc/rfc/spec-bosch-sieve-extprograms.txt)
    if (capability == "vnd.dovecot.pipe") {
        _command_map["pipe"] = true;
        _tag_map[":try"] = true;
    }

    // Hints
    _require_lookup["pipe"] = "vnd.dovecot.pipe";
    _require_lookup[":try"] = "vnd.dovecot.pipe";

    // vnd.dovecot.filter
    if (capability == "vnd.dovecot.filter") {
        _command_map["filter"] = true;
        _test_map["filter"] = true;
    }

    // Hint
    _require_lookup["filter"] = "vnd.dovecot.filter";

    // vnd.dovecot.execute
    if (capability == "vnd.dovecot.execute") {
        _command_map["execute"] = true;
        _test_map["execute"] = true;
        _tag_map[":input"] = true;
        _tag_map[":output"] = true;
        _tag_map[":pipe"] = true;
    }

    // Hint
    _require_lookup["execute"] = "vnd.dovecot.execute";
    _require_lookup[":input"] = "vnd.dovecot.execute";
    _require_lookup[":output"] = "vnd.dovecot.execute";
    _require_lookup[":pipe"] = "vnd.dovecot.execute";
}

} // namespace sieve
