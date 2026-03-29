require "fileinto";

if header :matches "Subject" "*money*fast*" {
    fileinto "spam";
}
