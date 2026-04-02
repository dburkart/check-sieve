# RFC 5232 §6 — hasflag tests a named variable rather than the implicit variable.
require ["imap4flags", "fileinto", "variables"];

addflag "myflags" "\\Flagged";
if hasflag "myflags" "\\Flagged" {
    fileinto "Flagged";
}
