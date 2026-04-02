# RFC 5232 §5 — :flags tag on fileinto specifies explicit flags for delivery.
require ["imap4flags", "fileinto"];

fileinto :flags "\\Deleted" "Trash";
