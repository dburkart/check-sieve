# RFC 5232 §6 — hasflag returns false when no flag matches.
require ["imap4flags", "fileinto"];

setflag "\\Flagged";
if hasflag :is "\\Answered" {
    fileinto "Answered";
}
