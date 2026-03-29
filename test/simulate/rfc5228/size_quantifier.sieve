# RFC 5228 §8.1 — size test with K numeric quantifier (1K = 1024 bytes).
# Verifies that the K suffix is correctly applied: 1K means 1024, not 1000.
# All test emails are well under 1024 bytes, so :under 1K always matches and
# :over 1K never does.
require "fileinto";

if size :under 1K {
    fileinto "tiny";
}

if size :over 1K {
    fileinto "large";
}
