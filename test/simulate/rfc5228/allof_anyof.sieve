# RFC 5228 §5.5/§5.6 — allof and anyof logical combinators.
# allof requires ALL sub-tests to be true (short-circuits on first false).
# anyof requires AT LEAST ONE sub-test to be true (short-circuits on first true).
# Two independent if blocks to show both combinators can fire in the same script.
require "fileinto";

if allof (header :contains "Subject" "money", exists "X-Spam-Score") {
    fileinto "definite-spam";
}

if anyof (header :is "From" "nobody@nowhere.com", exists "Date") {
    fileinto "has-date-or-from-nobody";
}
