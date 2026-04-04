require ["date", "index", "relational", "fileinto"];
if date :value "eq" :index 1 "received" "hour" "20" {
    fileinto "first-received-8pm";
}
