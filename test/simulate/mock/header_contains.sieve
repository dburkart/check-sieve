require "fileinto";

if header :contains "Subject" "university" {
    fileinto "junk";
}
