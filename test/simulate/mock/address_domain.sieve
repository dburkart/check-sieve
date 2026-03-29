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
