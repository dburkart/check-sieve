# RFC 5229 §3 — basic set and variable interpolation in fileinto.
require ["variables", "fileinto"];

set "folder" "inbox-archive";
fileinto "${folder}";
