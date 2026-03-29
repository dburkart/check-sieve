# RFC 5228 §5.4 — header test with :contains match-type.
# Verifies substring matching on a header value (case-insensitive by default).
# Matches any email whose Subject contains "university".
require "fileinto";

if header :contains "Subject" "university" {
    fileinto "junk";
}
