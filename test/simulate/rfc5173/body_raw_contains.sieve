# RFC 5173 §5 — body test with :raw transform and :contains match-type.
# Matches the entire undecoded body as a single string.
require "body";

if body :raw :contains "MAKE MONEY FAST" {
    discard;
}
