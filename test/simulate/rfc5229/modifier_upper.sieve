# RFC 5229 §4 — :upper modifier converts value to uppercase.
require ["variables", "fileinto"];

set :upper "tag" "hello";
fileinto "${tag}";
