# RFC 5229 §4 — :quotewildcard escapes wildcards so they can be used in :matches.
require ["variables", "fileinto"];

set :quotewildcard "escaped" "foo*bar";
fileinto "${escaped}";
