require "fileinto";

if header :matches "Subject" "test \* subject" {
    fileinto "literal-star";
}
