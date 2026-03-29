require "fileinto";

if allof (header :contains "Subject" "money", exists "X-Spam-Score") {
    fileinto "definite-spam";
}

if anyof (header :is "From" "nobody@nowhere.com", exists "Date") {
    fileinto "has-date-or-from-nobody";
}
