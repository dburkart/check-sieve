require ["subaddress", "fileinto"];
if address :detail :is "To" "detail" {
    fileinto "matched-detail";
}
