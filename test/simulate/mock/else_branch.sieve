require "fileinto";

if header :is "From" "nobody@nowhere.com" {
    fileinto "nobody";
} elsif header :is "From" "also-nobody@nowhere.com" {
    fileinto "also-nobody";
} else {
    fileinto "default";
}
