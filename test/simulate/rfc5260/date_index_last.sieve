require ["date", "index", "relational", "fileinto"];
if date :value "eq" :index 1 :last "received" "hour" "23" {
    fileinto "last-received-11pm-utc";
}
