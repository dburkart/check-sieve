# RFC 5228 §5.9 — size test with :under and :over, using plain numeric literals.
# Two independent tests run on every email: a message is "small" if under 1000
# bytes and "not-tiny" if over 100 bytes.  The test emails fall in both ranges,
# so both actions fire.  Verifies that :under and :over are strict inequalities.
require "fileinto";

if size :under 1000 {
    fileinto "small";
}

if size :over 100 {
    fileinto "not-tiny";
}
