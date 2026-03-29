require "fileinto";

if size :under 1000 {
    fileinto "small";
}

if size :over 100 {
    fileinto "not-tiny";
}
