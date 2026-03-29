require "fileinto";

if header :is "Subject" "this will never match anything" {
    fileinto "nope";
}
