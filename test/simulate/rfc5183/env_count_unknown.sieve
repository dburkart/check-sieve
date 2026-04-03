require ["environment", "fileinto"];
# "host" is "mail.example.com" — :matches wildcard should match
if environment :matches "host" "*.example.com" {
    fileinto "LocalHost";
}
