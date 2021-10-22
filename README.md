[![Build Status](https://travis-ci.org/dburkart/check-sieve.svg?branch=master)](https://travis-ci.org/dburkart/check-sieve)

### check-sieve

This project aims to make checking mail sieve syntax easy and painless. Because breaking your sieve in production sucks.

For a web-based demonstration of check-sieve which is entirely browser-based, visit https://checksieve.com.

#### Build Dependencies

`check-sieve` minimally depends on having a modern C++ compiler. On BSD-like systems such as FreeBSD, you'll
also need gmake installed. 

#### Development Dependencies

The following dependencies are required in order to develop check-sieve:

* bison >= v3.0.4
* flex >= v2.5.35
* Python >= 3.7
* Python development headers (for testing)

Additionally, if you want to generate a javascript library, you'll need emscripten installed.

#### Compiling

At the moment, there are four targets for this project:

1. `check-sieve` -- This is a command-line program you can use to verify sieves.
2. `libchecksieve.a` -- A statically linked library for embedding in other programs (like `check-sieve`).
3. `checksieve.so` -- This is a python extension created for use by the unit tests in the test directory
4. `wasm` -- This builds a webassembly javascript library using emscripten.

To build `check-sieve` simply run `make`. This will also build the static library.
Running `make test` will compile `checksieve.so` and run the python tests in the tests directory.
Finally, running `make all` will run all of the other targets.

#### Installing

To install to `/usr/local/`, simply run the following:

```
make install
```

To install to a custom location, build and install with `INSTALL_PREFIX` set in your environment
to the desired location.

##### Packages

Archlinux: https://aur.archlinux.org/packages/check-sieve/

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
* Imap4flags Extension ([RFC 5232](https://tools.ietf.org/html/rfc5232))
* Editheader Extension ([RFC 5293](https://tools.ietf.org/html/rfc5293))
* Regex Extension ([DRAFT](https://tools.ietf.org/html/draft-ietf-sieve-regex-01))
* Spamtest and Virustest Extensions ([RFC 5235](https://tools.ietf.org/html/rfc5235))
* Extensions for Notifications ([RFC 5435](https://tools.ietf.org/html/rfc5435))
* Subaddress Extension ([RFC 5233](https://tools.ietf.org/html/rfc5233))
* Ihave Extension ([RFC 5463](https://tools.ietf.org/html/rfc5463))
* Environment Extension ([RFC 5183](https://tools.ietf.org/html/rfc5183))
* Sieve Notification Mechanism: mailto ([RFC 5436](https://tools.ietf.org/html/rfc5436))
* Internet Message Access Protocol (IMAP) Events ([RFC 6785](https://tools.ietf.org/html/rfc6785))
* Converting Messages before Delivery ([6558](https://tools.ietf.org/html/rfc6558))

If you find any discrepancies with these RFCs, or have reduced test-cases that should work but don't, please file an
issue. If there's an RFC that isn't supported that you want, file an issue!
