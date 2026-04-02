# RFC 5232 §4 — named variable with addflag; :flags tag on fileinto uses variable expansion.
require ["imap4flags", "fileinto", "variables"];

addflag "myflags" "\\Flagged";
fileinto :flags "${myflags}" "Archive";
