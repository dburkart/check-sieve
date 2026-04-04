require ["subaddress", "fileinto"];
if address :detail :is "From" "" {
    fileinto "should-not-match";
}
