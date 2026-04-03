require ["environment", "fileinto"];
# "phase" item is "during", not "post" — should not match
if environment :is "phase" "post" {
    fileinto "PostDelivery";
}
