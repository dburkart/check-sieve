require ["date", "relational", "fileinto"];
if currentdate :count "eq" :comparator "i;ascii-numeric" "year" "1" {
    fileinto "date-available";
}
