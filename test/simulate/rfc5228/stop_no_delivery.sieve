# RFC 5228 §3.3/§4.1 — stop with no prior delivery action triggers implicit keep.
# Per RFC §4.2, if no delivery action has executed when stop is reached, the
# implicit keep still fires.  Verifies that stop alone does not silently drop
# the message.
stop;
