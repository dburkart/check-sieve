# RFC 5173 §5 — body test with :content and specific type/subtype pattern.
# "text/plain" matches only text/plain parts, not text/html.
require ["body", "fileinto"];

if body :content "text/plain" :contains "plain text part" {
    fileinto "plaintext-messages";
}
