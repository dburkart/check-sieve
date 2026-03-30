# RFC 5173 §5 — body test that never matches.
# Verifies that a non-matching body test results in implicit keep.
require "body";

if body :raw :contains "this string is not present anywhere" {
    discard;
}
