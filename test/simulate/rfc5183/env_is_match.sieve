require ["environment", "fileinto"];
# "name" item is "check-sieve" — :is match should succeed
if environment :is "name" "check-sieve" {
    fileinto "Matched";
}
