# RFC 5232 §6 — hasflag on a named variable does not match the implicit variable.
require ["imap4flags", "fileinto", "variables"];

setflag "\\Flagged";
if hasflag "myflags" "\\Flagged" {
    fileinto "Should-Not-Match";
}
