# RFC 5229 §3.2 — :matches populates ${1}–${9} with wildcard captures.
require ["variables", "fileinto"];

if header :matches "Subject" "Hello *" {
    fileinto "matched-${1}";
}
