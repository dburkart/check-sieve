[![Build Status](https://travis-ci.org/dburkart/mail-sieve-verifier.svg?branch=master)](https://travis-ci.org/dburkart/mail-sieve-verifier)

### mail-sieve-verifier 

This project aims to make checking mail sieve syntax easy and painless. Because breaking your sieve in production sucks.

#### Dependencies

`mail-sieve-verifier` minimally depends on having a modern C++ compiler, however there are some requirements
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