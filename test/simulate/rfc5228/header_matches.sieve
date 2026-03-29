# RFC 5228 §5.4 / §2.7.3 — header test with :matches glob match-type.
# Verifies wildcard matching: * matches zero or more characters.
# The pattern "*money*fast*" matches any Subject containing both substrings
# in order, regardless of surrounding text.
require "fileinto";

if header :matches "Subject" "*money*fast*" {
    fileinto "spam";
}
