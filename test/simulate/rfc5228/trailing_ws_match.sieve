# RFC 5228 §5.4 — header value whitespace stripping.
# Per the RFC, leading and trailing whitespace must be stripped from header
# values before comparison.  trailing_ws.eml has "Subject: hello   " (trailing
# spaces); :is "hello" must still match after stripping.
if header :is "Subject" "hello" {
    keep;
}
