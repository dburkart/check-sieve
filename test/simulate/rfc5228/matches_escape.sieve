# RFC 5228 §2.7.3 — :matches with backslash-escaped wildcard characters.
# \* is a literal asterisk, not a glob wildcard.  Only emails whose Subject
# contains an actual * character should match.  Verifies that escaped
# wildcards are not treated as pattern operators.
require "fileinto";

if header :matches "Subject" "test \* subject" {
    fileinto "literal-star";
}
