#include <algorithm>
#include <iostream>
#include <vector>

#include "ASTVerificationVisitor.hh"

namespace sieve
{

ASTVerificationVisitor::ASTVerificationVisitor(struct parse_options options)
    : _options( options )
    , _verification_result()
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
        return;
    }

    auto r = _command.validate(node);
    if (!r.result()) {
        auto hint = _command.usage(node);

        if (r.hint() != "") {
            if (r.hint_as_error()) {
                hint = r.hint();
            } else {
                hint += "\n\n(Hint: " + r.hint() + ")";
            }
        }

        _verification_result = {
            1,
            node->location(),
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
        _capability_map[child->value()] = 1;
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
        return;
    }

    auto r =  _tag.validate(node);
    if (!r.result()) {
        auto hint = _tag.usage(node);

        if (r.hint() != "") {
            if (r.hint_as_error()) {
                hint = r.hint();
            } else {
                hint += "\n\n(Hint: " + r.hint() + ")";
            }
        }

        _verification_result = {
            1,
            node->location(),
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

        if (r.hint() != "") {
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

    // "body"
    // RFC 5173
    if (capability == "body") {
        _test_map["body"] = true;
        _tag_map[":raw"] = true;
        _tag_map[":content"] = true;
        _tag_map[":text"] = true;
    }

    // "environment"
    // RFC 5183
    if (capability == "environment") {
        _test_map["environment"] = true;
    }

    // "fileinto"
    // RFC 5228
    if (capability == "fileinto") {
        _command_map["fileinto"] = true;
    }

    // "reject"
    // RFC 5228
    if (capability == "reject") {
        _command_map["reject"] = true;
    }

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

    // "relational"
    // RFC 5231
    if (capability == "relational") {
        _tag_map[":count"] = true;
        _tag_map[":value"] = true;
    }

    // "imap4flags"
    // RFC 5232
    if (capability == "imap4flags") {
        _command_map["setflag"] = true;
        _command_map["addflag"] = true;
        _command_map["removeflag"] = true;
        _test_map["hasflag"] = true;
        _tag_map[":flags"] = true;
    }

    // "subaddress"
    // RFC 5233
    if (capability == "subaddress") {
        _tag_map[":user"] = true;
        _tag_map[":detail"] = true;
    }

    // "date"
    // RFC 5260
    if (capability == "date") {
        _test_map["date"] = true;
        _test_map["currentdate"] = true;
        _tag_map[":zone"] = true;
        _tag_map[":originalzone"] = true;
    }

    // "index"
    // RFC 5260
    if (capability == "index") {
        _tag_map[":index"] = true;
        _tag_map[":last"] = true;
    }

    // "spamtest" or "spamtestplus"
    // RFC 5235
    if (capability == "spamtest" || capability == "spamtestplus") {
        _test_map["spamtest"] = true;
        _tag_map[":percent"] = true;
    }

    // "virustest"
    // RFC 5235
    if (capability == "virustest") {
        _test_map["virustest"] = true;
    }

    // "ereject"
    // RFC 5429
    if (capability == "ereject") {
        _command_map["ereject"] = true;
    }

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
    }

    // "ihave"
    // RFC 5463
    if (capability == "ihave") {
        _command_map["error"] = true;
        _test_map["ihave"] = true;
    }

    // "mailbox"
    // RFC 5490
    if (capability == "mailbox") {
        _test_map["mailboxexists"] = true;
        _tag_map[":create"] = true;
    }

    // "mboxmetadata"
    // RFC 5490
    if (capability == "mboxmetadata") {
        _test_map["metadata"] = true;
        _test_map["metadataexists"] = true;
    }

    // "servermetadata"
    // RFC 5490
    if (capability == "servermetadata") {
        _test_map["servermetadata"] = true;
        _test_map["servermetadataexists"] = true;
    }

    // "foreverypart"
    // RFC 5703
    if (capability == "foreverypart") {
        _command_map["foreverypart"] = true;
        _command_map["break"] = true;
        _tag_map[":name"] = true;
    }

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

    // "extracttext"
    // RFC 5703
    if (capability == "extracttext") {
        _command_map["extracttext"] = true;
        _tag_map[":first"] = true;
    }

    // "replace"
    // RFC 5703
    if (capability == "replace") {
        _command_map["replace"] = true;
        _tag_map[":subject"] = true;
        _tag_map[":from"] = true;
    }

    // "enclose"
    // RFC 5703
    if (capability == "enclose") {
        _command_map["enclose"] = true;
        _tag_map[":subject"] = true;
        _tag_map[":headers"] = true;
    }

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
    }

    // "convert"
    // RFC 6658
    if (capability == "convert") {
        _command_map["convert"] = true;
        _test_map["convert"] = true;
    }

    // "extlists"
    // RFC 6134
    if (capability == "extlists") {
        _tag_map[":list"] = true;
        _test_map["valid_ext_list"] = true;
    }

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

    // "special-use"
    // RFC 8579
    // https://www.rfc-editor.org/rfc/rfc8579
    if (capability == "special-use") {
        _test_map["specialuse_exists"] = true;
        _tag_map[":specialuse"] = true;
    }

    // "fcc"
    // RFC 8580
    // https://www.rfc-editor.org/rfc/rfc8580.html
    if (capability == "fcc") {
        _tag_map[":fcc"] = true;
    }

    // "mailboxid"
    // RFC 9042
    // https://www.rfc-editor.org/rfc/rfc9042.html
    if (capability == "mailboxid") {
        _test_map["mailboxidexists"] = true;
        _tag_map[":mailboxid"] = true;
    }

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
    }

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

    // "vnd.proton.eval"
    // (https://proton.me/support/sieve-advanced-custom-filters#transforming-variables)
    // depends on "variables"
    if (capability == "vnd.proton.eval" && has_required("variables")) {
        _tag_map[":eval"] = true;
    }
}

} // namespace sieve
