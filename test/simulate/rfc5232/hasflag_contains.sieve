# RFC 5232 §6 — hasflag :contains matches flags by substring.
require ["imap4flags", "fileinto"];

setflag "\\Flagged";
if hasflag :contains "lag" {
    fileinto "Matched";
}
