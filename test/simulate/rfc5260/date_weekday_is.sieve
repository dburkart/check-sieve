require ["date", "fileinto"];
if date :is "date" "weekday" "6" {
    fileinto "weekend";
}
