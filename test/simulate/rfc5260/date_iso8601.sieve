require ["date", "fileinto"];
if date :is "date" "iso8601" "2025-03-01T14:30:00Z" {
    fileinto "exact-time";
}
