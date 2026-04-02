# RFC 5229 + RFC 5232 — use set to build a flag value, then pass it to setflag.
require ["imap4flags", "variables"];

set "flagname" "\\Flagged";
setflag "${flagname}";
