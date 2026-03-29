# RFC 5228 §5.1 / §2.7.4 — :localpart with a syntactically invalid address.
# When the From header contains a bare word with no @, :localpart must return
# an empty string rather than the whole value, so the test never matches.
# Verifies that invalid addresses fail :localpart tests as the RFC requires.
if address :localpart :is "From" "justlocal" {
    discard;
}
