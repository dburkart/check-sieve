require ["date", "relational", "fileinto"];
if date :value "ge" :originalzone "date" "hour" "14" {
    fileinto "afternoon";
}
