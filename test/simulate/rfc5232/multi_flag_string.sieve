# RFC 5232 §4 — space-separated flags within a single string are equivalent to a list.
require "imap4flags";

addflag "\\Flagged \\Answered";
