require "fileinto";

if exists "X-Spam-Score" {
    fileinto "spam";
    stop;
}

if size :over 1000000 {
    fileinto "large";
}
