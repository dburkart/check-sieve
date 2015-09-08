[![Build Status](https://travis-ci.org/dburkart/check-sieve.svg?branch=master)](https://travis-ci.org/dburkart/check-sieve)

### check-sieve

This project aims to make checking mail sieve syntax easy and painless. Because breaking your sieve in production sucks.

#### Dependencies

`check-sieve` minimally depends on having a modern C++ compiler, however there are some requirements
for developing on the project:

* bison >= v3.0.2
* flex >= v2.5.35
* python development headers (for testing)

#### Compiling

At the moment, there are three targets for this project:

1. `check-sieve` -- This is a command-line program you can use to verify sieves.
2. `libchecksieve.a` -- A statically linked library for embedding in other programs (like `check-sieve`).
3. `checksieve.so` -- This is a python extension created for use by the unit tests in the test directory

To build `check-sieve` simply run `make`. This will also build the static library.
Running `make test` will compile `checksieve.so` and run the python tests in the tests directory.
Finally, running `make all` will run all of the other targets.

#### Supported RFCs

Currently, the supported RFCs are:

* Sieve: An Email Filtering Language ([RFC 5228](https://tools.ietf.org/html/rfc5228))
* Variables Extension ([RFC 5229](https://tools.ietf.org/html/rfc5229))
* Date and Index Extensions ([RFC 5260](https://tools.ietf.org/html/rfc5260))
* Vacation Extension ([RFC 5230](https://tools.ietf.org/html/rfc5230))
* Include Extension ([RFC 6609](https://tools.ietf.org/html/rfc6609))
* Copying Without Side Effects ([RFC 3894](https://tools.ietf.org/html/rfc3894))
* Reject and Extended Reject Extensions ([RFC 5429](https://tools.ietf.org/html/rfc5429))
* Body Extension ([RFC 5173](https://tools.ietf.org/html/rfc5173))
* Relational Extension ([RFC 5231](https://tools.ietf.org/html/rfc5231))
* MIME Part Tests, Iteration, Extraction, Replacement, and Enclosure ([RFC 5703](https://tools.ietf.org/html/rfc5703))
* Imap4flags Extension ([RFC 5231](https://tools.ietf.org/html/rfc5232))

If you find any discrepancies with these RFCs, or have reduced test-cases that should work but don't, please file an
issue on the issues tab. If an RFC is missing which isn't supported, that you would like supported, also file an issue!
