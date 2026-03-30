# RFC 5229 §3.2 — match variables are cleared before each top-level test.
# The second if uses :is (no match), so ${1} should be empty.
require ["variables", "fileinto"];

if header :matches "Subject" "Hello *" {
    fileinto "first-${1}";
}
if header :is "Subject" "no-match" {
    fileinto "second-${1}";
}
fileinto "after-${1}";
