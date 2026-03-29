# RFC 5228 §5.1 — address test, :localpart address-part.
# Verifies that :localpart extracts only the left-hand side of an address
# (before the @) for exact matching.
require "fileinto";

if address :localpart :is "From" "john" {
    fileinto "known";
}
