require ["date", "relational", "fileinto"];
if date :value "eq" :zone "-0500" "date" "hour" "09" {
    fileinto "eastern-morning";
}
