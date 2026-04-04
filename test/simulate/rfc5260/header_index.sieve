require ["index", "fileinto"];
if header :index 1 :contains "received" "abc123" {
    fileinto "first-hop";
}
