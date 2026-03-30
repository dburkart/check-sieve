# RFC 5229 §4 — :lower modifier converts value to lowercase.
require ["variables", "fileinto"];

set :lower "tag" "HELLO";
fileinto "${tag}";
