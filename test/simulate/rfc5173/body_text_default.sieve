# RFC 5173 §5 — body test with default :text transform.
# Matches text/* parts of the message body.
require ["body", "fileinto"];

if body :contains "plain text part" {
    fileinto "text-messages";
}
