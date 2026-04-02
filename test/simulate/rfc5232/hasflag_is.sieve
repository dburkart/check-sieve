# RFC 5232 §6 — hasflag :is tests whether the implicit variable contains a flag exactly.
require ["imap4flags", "fileinto"];

setflag "\\Flagged";
if hasflag :is "\\Flagged" {
    fileinto "Flagged";
}
