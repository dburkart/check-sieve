# RFC 5229 §4 — :length modifier stores the string length as a decimal number.
require ["variables", "fileinto"];

set :length "len" "hello";
fileinto "length-${len}";
