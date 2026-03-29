require "fileinto";

if address :localpart :is "From" "john" {
    fileinto "known";
}
