# RFC 5228 §4.4/§4.5 — redirect and discard actions.
# redirect forwards the message to another address and cancels implicit keep.
# discard silently drops the message and also cancels implicit keep.
# The else branch ensures discard fires when the condition is not met.
require "fileinto";

if exists "X-Spam-Score" {
    redirect "admin@example.com";
} else {
    discard;
}
