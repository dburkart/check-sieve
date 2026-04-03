require ["environment", "fileinto"];
# "host" is "mail.example.com" — :contains "example" should match
if environment :contains "host" "example" {
    fileinto "Internal";
}
