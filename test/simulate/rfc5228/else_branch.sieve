# RFC 5228 §3.1 — if/elsif/else control flow.
# Verifies that exactly one branch fires: if no condition matches, the else
# block executes.  Neither From address appears in any test email, so every
# email exercises the else path.
require "fileinto";

if header :is "From" "nobody@nowhere.com" {
    fileinto "nobody";
} elsif header :is "From" "also-nobody@nowhere.com" {
    fileinto "also-nobody";
} else {
    fileinto "default";
}
