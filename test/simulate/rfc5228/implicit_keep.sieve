# RFC 5228 §4.1 — implicit keep.
# When no action command executes, the message is implicitly kept in the
# default mailbox.  The condition here is designed to never match any test
# email, so all inputs exercise the implicit keep path.
require "fileinto";

if header :is "Subject" "this will never match anything" {
    fileinto "nope";
}
