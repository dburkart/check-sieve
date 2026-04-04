require ["date", "relational", "fileinto"];
if date :count "eq" :comparator "i;ascii-numeric" "date" "year" "1" {
    fileinto "has-date";
}
