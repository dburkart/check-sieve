require "fileinto";

if size :under 1K {
    fileinto "tiny";
}

if size :over 1K {
    fileinto "large";
}
