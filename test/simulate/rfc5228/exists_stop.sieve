# RFC 5228 §5.8 / §3.3 — exists test and stop command.
# Verifies that stop halts processing: the second if (size :over 1000000)
# is never evaluated for emails that have X-Spam-Score.  For emails without
# that header, stop is not reached and the size check applies (but no email
# in the test suite is over 1 MB, so implicit keep fires).
require "fileinto";

if exists "X-Spam-Score" {
    fileinto "spam";
    stop;
}

if size :over 1000000 {
    fileinto "large";
}
