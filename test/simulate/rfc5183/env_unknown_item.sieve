require ["environment", "fileinto"];
# "location" item is "MTA" — checking a different item than env_is_match
if environment :is "location" "MTA" {
    fileinto "ViaMailTransfer";
}
