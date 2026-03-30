# RFC 5173 §5 — body test with :content and type-only pattern.
# "text" matches any text/* part (text/plain, text/html, etc.).
require ["body", "fileinto"];

if body :content "text" :contains "html" {
    fileinto "html-messages";
}
