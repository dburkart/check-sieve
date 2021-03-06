#include <algorithm>
#include <iostream>
#include <vector>

#include "ASTVerificationVisitor.hh"

namespace sieve
{

ASTVerificationVisitor::ASTVerificationVisitor(struct parse_options options)
    : _verification_result()
    , _command()
    , _test()
    , _tag()
    , _options( options )
    , _required_capabilities( NULL ) {
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
    for (std::vector<sieve::ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        sieve::ASTNode *child = *it;
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
    }

    else if (!_command.validate(node)) {
        _verification_result = {
            1,
            node->location(),
            "Incorrect syntax for command \"" + node->value() + "\".",
            _command.usage(node)
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
    if (dynamic_cast<ASTStringList *>(children[0]) != NULL) {
        _required_capabilities = dynamic_cast<ASTStringList *>(children[0]);
        children = children[0]->children();
    }

    for (std::vector<sieve::ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        sieve::ASTString *child = static_cast<ASTString *>(*it);
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
    }

    if (!_tag.validate(node)) {
        _verification_result = {
            1,
            node->location(),
            "Incorrect syntax for tag \"" + node->value() + "\".",
            _tag.usage(node)
        };
    }
}

void ASTVerificationVisitor::visit( ASTTest* node ) {
    std::vector<sieve::ASTNode *> children = node->children();
    std::string value_lower = node->value();
    std::transform(value_lower.begin(), value_lower.end(), value_lower.begin(), ::tolower);
    const ASTTag *first_match_tag = NULL;
    const ASTTag *second_match_tag = NULL;

    if (!_test_map[value_lower]) {
        _verification_result = {1, node->location(), "Unrecognized test \"" + node->value() + "\"."};
    }

    for (std::vector<sieve::ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        // Are we an ASTTag?
        const ASTTag *child = dynamic_cast<ASTTag*>(*it);

        if (child != NULL) {
            // Ensure that we have only _one_ of :is, :contains, or :matches
            if (child->value() == ":is" || child->value() == ":contains" || child->value() == ":matches") {
                if (first_match_tag == NULL)
                    first_match_tag = child;
                else if (second_match_tag == NULL)
                    second_match_tag = child;
            }
        }

        // If it's an "ihave" test, we need to enable any specified capabilities
        if (value_lower == "ihave") {
            const ASTString *capability = dynamic_cast<ASTString*>(*it);

            if (capability != NULL) {
                _enable_capability(capability->value());
            }
        }
    }

    if (second_match_tag != NULL) {
        _verification_result = {1, second_match_tag->location(), "Only one match type tag is allowed; first match type tag was \"" + first_match_tag->value() + "\"."};
    }

    if (!_test.validate(node)) {
        _verification_result = {
            1,
            node->location(),
            "Incorrect syntax for test \"" + node->value() + "\".",
            _test.usage(node)
        };
    }
}

//-- Private methods
void ASTVerificationVisitor::_init() {
    _command_map["addheader"] = 1;
    _command_map["keep"] = 1;
    _command_map["deleteheader"] = 1;
    _command_map["discard"] = 1;
    _command_map["redirect"] = 1;
    _command_map["stop"] = 1;

    _test_map["allof"] = 1;
    _test_map["anyof"] = 1;
    _test_map["address"] = 1;
    _test_map["envelope"] = 1;
    _test_map["header"] = 1;
    _test_map["size"] = 1;
    _test_map["not"] = 1;
    _test_map["exists"] = 1;
    _test_map["$command_result"] = 1; // Special test indicating the result of a command

    _tag_map[":is"] = 1;
    _tag_map[":contains"] = 1;
    _tag_map[":matches"] = 1;
    _tag_map[":last"] = 1;
    _tag_map[":localpart"] = 1;
    _tag_map[":domain"] = 1;
    _tag_map[":all"] = 1;
    _tag_map[":over"] = 1;
    _tag_map[":under"] = 1;

    // TODO: "Comparators other than "i;octet" and "i;ascii-casemap" must be
    // declared with require, as they are extensions"
    _tag_map[":comparator"] = 1;
    
    // Set up references to this visitor for introspection
    _command.set_visitor(this);
    _test.set_visitor(this);
    _tag.set_visitor(this);
    
}

void ASTVerificationVisitor::_enable_capability(std::string capability) {
    // "copy"
    // RFC 3894
    if (capability == "copy") {
        _tag_map[":copy"] = 1;
    }

    // "body"
    // RFC 5173
    if (capability == "body") {
        _test_map["body"] = 1;
        _tag_map[":raw"] = 1;
        _tag_map[":content"] = 1;
        _tag_map[":text"] = 1;
    }

    // "environment"
    // RFC 5183
    if (capability == "environment") {
        _test_map["environment"] = 1;
    }

    // "fileinto"
    // RFC 5228
    if (capability == "fileinto") {
        _command_map["fileinto"] = 1;
    }

    // "reject"
    // RFC 5228
    if (capability == "reject") {
        _command_map["reject"] = 1;
    }

    // "variables"
    // RFC 5229
    if (capability == "variables") {
        _command_map["set"] = 1;
        _test_map["string"] = 1;
        _tag_map[":lower"] = 1;
        _tag_map[":upper"] = 1;
        _tag_map[":lowerfirst"] = 1;
        _tag_map[":upperfirst"] = 1;
        _tag_map[":quotewildcard"] = 1;
        _tag_map[":length"] = 1;
    }

    // "vacation"
    // RFC 5230
    if (capability == "vacation") {
        _command_map["vacation"] = 1;
        _tag_map[":days"] = 1;
        _tag_map[":from"] = 1;
        _tag_map[":subject"] = 1;
        _tag_map[":addresses"] = 1;
        _tag_map[":mime"] = 1;
        _tag_map[":handle"] = 1;
    }

    // "relational"
    // RFC 5231
    if (capability == "relational") {
        _tag_map[":count"] = 1;
        _tag_map[":value"] = 1;
    }

    // "imap4flags"
    // RFC 5232
    if (capability == "imap4flags") {
        _command_map["setflag"] = 1;
        _command_map["addflag"] = 1;
        _command_map["removeflag"] = 1;
        _test_map["hasflag"] = 1;
        _tag_map[":flags"] = 1;
    }

    // "subaddress"
    // RFC 5233
    if (capability == "subaddress") {
        _tag_map[":user"] = 1;
        _tag_map[":detail"] = 1;
    }

    // "date"
    // RFC 5260
    if (capability == "date") {
        _test_map["date"] = 1;
        _test_map["currentdate"] = 1;
        _tag_map[":zone"] = 1;
        _tag_map[":originalzone"] = 1;
    }

    // "index"
    // RFC 5260
    if (capability == "index") {
        _tag_map[":index"] = 1;
        _tag_map[":last"] = 1;
    }

    // "spamtest" or "spamtestplus"
    // RFC 5235
    if (capability == "spamtest" || capability == "spamtestplus") {
        _test_map["spamtest"] = 1;
        _tag_map[":percent"] = 1;
    }

    // "virustest"
    // RFC 5235
    if (capability == "virustest") {
        _test_map["virustest"] = 1;
    }

    // "ereject"
    // RFC 5429
    if (capability == "ereject") {
        _command_map["ereject"] = 1;
    }

    // "enotify"
    // RFC 5435
    if (capability == "enotify") {
        _command_map["notify"] = 1;
        _tag_map[":from"] = 1;
        _tag_map[":importance"] = 1;
        _tag_map[":options"] = 1;
        _tag_map[":message"] = 1;
        _test_map["valid_notify_method"] = 1;
        _test_map["notify_method_capability"] = 1;

        // The :encodeurl tag can only be used if both "enotify" and
        // "variables" are required
        if (_required_capabilities != NULL &&
            _required_capabilities->find(ASTString("variables")) != _required_capabilities->children().end()) {
            _tag_map[":encodeurl"] = 1;
        }
    }

    // "ihave"
    // RFC 5463
    if (capability == "ihave") {
        _command_map["error"] = 1;
        _test_map["ihave"] = 1;
    }

    // "mailbox"
    // RFC 5490
    if (capability == "mailbox") {
        _test_map["mailboxexists"] = 1;
        _tag_map[":create"] = 1;
    }

    // "mboxmetadata"
    // RFC 5490
    if (capability == "mboxmetadata") {
        _test_map["metadata"] = 1;
        _test_map["metadataexists"] =1;
    }

    // "servermetadata"
    // RFC 5490
    if (capability == "servermetadata") {
        _test_map["servermetadata"] = 1;
        _test_map["servermetadataexists"] = 1;
    }

    // "foreverypart"
    // RFC 5703
    if (capability == "foreverypart") {
        _command_map["foreverypart"] = 1;
        _command_map["break"] = 1;
        _tag_map[":name"] = 1;
    }

    // "mime"
    // RFC 5703
    if (capability == "mime") {
        _tag_map[":mime"] = 1;
        _tag_map[":type"] = 1;
        _tag_map[":subtype"] = 1;
        _tag_map[":contenttype"] = 1;
        _tag_map[":param"] = 1;
        _tag_map[":anychild"] = 1;
    }

    // "extracttext"
    // RFC 5703
    if (capability == "extracttext") {
        _command_map["extracttext"] = 1;
        _tag_map[":first"] = 1;
    }

    // "replace"
    // RFC 5703
    if (capability == "replace") {
        _command_map["replace"] = 1;
        _tag_map[":subject"] = 1;
        _tag_map[":from"] = 1;
    }

    // "enclose"
    // RFC 5703
    if (capability == "enclose") {
        _command_map["enclose"] = 1;
        _tag_map[":subject"] = 1;
        _tag_map[":headers"] = 1;
    }

    // "include"
    // RFC 6609
    if (capability == "include") {
        _command_map["include"] = 1;
        _command_map["return"] = 1;
        _tag_map[":once"] = 1;
        _tag_map[":optional"] = 1;
        _tag_map[":personal"] = 1;
        _tag_map[":global"] = 1;

        // The "global" command can only be used if both "include" and
        // "variables" are required
        if (_required_capabilities != NULL &&
            _required_capabilities->find(ASTString("variables")) != _required_capabilities->children().end()) {
            _command_map["global"] = 1;
        }
    }
    
    // "convert"
    // RFC 6658
    if (capability == "convert") {
        _command_map["convert"] = 1;
    }

    // DRAFT RFCs

    // "regex"
    // (https://tools.ietf.org/html/draft-ietf-sieve-regex-01)
    if (capability == "regex") {
        _tag_map[":regex"] = 1;

        // The ":quoteregex" command is supported if both "regex" and
        // "variables" are required
        if (_required_capabilities != NULL &&
            _required_capabilities->find(ASTString("variables")) != _required_capabilities->children().end()) {
            _tag_map[":quoteregex"] = 1;
        }
    }
}

} // namespace sieve
