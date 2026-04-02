# RFC 5232 §4 — removeflag removes a flag from the implicit variable.
require "imap4flags";

addflag "\\Flagged";
addflag "\\Answered";
removeflag "\\Flagged";
