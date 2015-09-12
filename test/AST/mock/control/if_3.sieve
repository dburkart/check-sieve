require "fileinto";

if true {
    keep;
    stop;
} elsif false {
    fileinto "\\Seen";
    stop;
} else {
    stop;
}