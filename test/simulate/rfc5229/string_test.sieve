# RFC 5229 §5 — string test compares script values with variable expansion.
require ["variables", "fileinto"];

set "greeting" "Hello World";
if string :contains "${greeting}" "World" {
    fileinto "greetings";
}
