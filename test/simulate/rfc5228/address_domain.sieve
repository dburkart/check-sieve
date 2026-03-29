# RFC 5228 §5.1 — address test, :domain address-part.
# Verifies that :domain extracts and matches the right-hand side of an
# address across an if/elsif/else chain.  The anyof fallback branch also
# exercises :all :contains and :matches with a string-list pattern.
require "fileinto";

if header :is "Sender" "owner-ietf-mta-filters@imc.org"
{
    fileinto "filter";
}
elsif address :domain :is ["From", "To"] "example.com"
{
    keep;
}
elsif anyof (
    not address :all :contains ["To", "Cc", "Bcc"] "me@example.com",
    header :matches "subject" ["*make*money*fast*", "*university*dipl*mas*"]
)
{
    fileinto "spam";
}
else
{
    fileinto "other";
}
