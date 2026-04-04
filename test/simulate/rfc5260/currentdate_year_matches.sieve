require ["date", "fileinto"];
if currentdate :matches "year" "????" {
    fileinto "has-year";
}
