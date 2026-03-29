# RFC 5228 §5.10/§5.11 — true and false boolean tests.
# true always matches; false never matches.  Verifies the simulator handles
# literal boolean tests and that a false branch is never executed.
require "fileinto";

if true {
    fileinto "always";
}

if false {
    fileinto "never";
}
