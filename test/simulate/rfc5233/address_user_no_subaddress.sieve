require ["subaddress", "fileinto"];
if address :user :is "From" "sender" {
    fileinto "matched-no-plus";
}
