require ["subaddress", "fileinto"];
if address :user :is "To" "user" {
    fileinto "matched-user";
}
