# RFC 5229 §3 — unknown variable references expand to empty string.
require ["variables", "fileinto"];

fileinto "box-${unset}";
