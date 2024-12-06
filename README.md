<div align="center">
    <h1>
        check-sieve
        <br>
        <em><sup><sub>
            Because breaking your sieve in production sucks.
        </sub></sup></em>
    </h1>
</div>

<div align="center">

[![badge-license]][url-license]
[![badge-contributors]][url-contributors]
![badge-language]
[![badge-tests]][url-tests]
[![badge-last-release]][url-last-release]

</div>

[badge-license]: https://img.shields.io/github/license/dburkart/check-sieve?style=for-the-badge
[badge-contributors]: https://img.shields.io/github/contributors/dburkart/check-sieve?style=for-the-badge
[badge-language]: https://img.shields.io/github/languages/top/dburkart/check-sieve?style=for-the-badge
[badge-tests]: https://img.shields.io/github/actions/workflow/status/dburkart/check-sieve/c-cpp.yml?style=for-the-badge&label=Tests
[badge-last-release]: https://img.shields.io/github/release-date/dburkart/check-sieve?style=for-the-badge&label=Last%20Release

[url-license]: ./LICENSE
[url-contributors]: https://github.com/dburkart/check-sieve/graphs/contributors
[url-tests]: https://github.com/dburkart/check-sieve/actions
[url-last-release]: https://github.com/dburkart/check-sieve/releases/latest


This project aims to make checking mail [Sieve][wiki-sieve] syntax easy and painless.
_Because breaking your sieve in production sucks._

[wiki-sieve]: https://en.wikipedia.org/wiki/Sieve_(mail_filtering_language)

Particular attention is paid to the quality of errors generated. For example:

```
Errors found in "testsieve":

Incorrect syntax for command "convert".
On line 2:
convert image image ["pix-x=320","pix-y=240"];
^^^^^^^
Usage: convert  <quoted-from-media-type: string>                                      
         <quoted-to-media-type: string>                                        
         <transcoding-params: string-list>  
```

For a web-based demonstration of check-sieve which is entirely browser-based, visit https://checksieve.com.

## Usage
```
Usage: check-sieve [options] file1 [file2 ...]                                 
                                                                               
Options:                                                                       
  -h, --help               Show this message                                   
  --max-list-length N      Flag lists over a certain length (default: none)    
  --server <host>:<port>   Only allow capabilities advertised by the specified 
                           server.                                             
  --trace-parser           Trace the operation of the parser                   
  --trace-scanner          Trace the operation of the scanner                  
  --trace-tree             Trace the abstract-syntax-tree                      
  --version                Print out version information 
```

## Features
### Supported RFCs
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
* Extensions for Checking Mailbox Status and Accessing Mailbox Metadata ([RFC 5490](https://tools.ietf.org/html/rfc5490))
* Internet Message Access Protocol (IMAP) Events ([RFC 6785](https://tools.ietf.org/html/rfc6785))
* Converting Messages before Delivery ([RFC 6558](https://tools.ietf.org/html/rfc6558))
* Detecting Duplicate Deliveries ([RFC 7352](https://tools.ietf.org/html/rfc7352))
* Delivering to Special-Use Mailboxes ([RFC 8579](https://tools.ietf.org/html/rfc8579))
* File Carbon Copy (FCC) ([RFC 8580](https://tools.ietf.org/html/rfc8580))
* Delivery by MAILBOXID ([RFC 9042](https://tools.ietf.org/html/rfc9042))
* Proton Expiration Extension ([vnd.proton.expire](https://proton.me/support/sieve-advanced-custom-filters#managing-expiration))
* Proton Eval Extension ([vnd.proton.eval](https://proton.me/support/sieve-advanced-custom-filters#transforming-variables))

> [!NOTE]
> If you find any discrepancies with these RFCs,
> have a request for an RFC not included,
> or have test-cases that should work but don't,
> please [file an issue][file-issue].

[file-issue]: https://github.com/dburkart/check-sieve/issues/new

## Development
### Dependencies
`check-sieve` minimally depends on having a modern C++ compiler and the
[Make][make] build automation software.

On BSD-like systems such as FreeBSD, the [`gmake`][gnu-make] tool is required.

[make]: https://en.wikipedia.org/wiki/Make_(software)
[gnu-make]: https://www.gnu.org/software/make/

Building `check-sieve` only depends on those aforementioned tools. Developing
`check-sieve` requires addition dependencies:

* [GNU Bison][bison] >= v3.0.4
* [Flex][flex] >= v2.5.35
* [Python][python] >= 3.7
* Python development headers (for testing)

Additionally, [Emscripten][emscripten] is required to generate the Check Sieve JavaScript library.

[bison]: https://www.gnu.org/software/bison/
[flex]: https://github.com/westes/flex
[python]: https://www.python.org/
[emscripten]: https://emscripten.org/

#### Installing dependencies
<details>
<summary><strong>Ubuntu and Debian-based distributions</strong></summary>
<p>

```bash
# Synchronize mirrors
sudo apt-get update

# Core development dependencies
sudo apt-get install make gcc g++ bison flex python3 libpython3-dev 

# JavaScript dependencies
sudo apt-get install emscripten
```

</p>
</details>

<details>
<summary><strong>Arch Linux</strong></summary>
<p>

```bash
# Synchronize mirrors, and
# Core development dependencies
sudo pacman -Syu make gcc bison flex python 

# JavaScript dependencies
sudo pacman -Sy emscripten
```

</p>
</details>


### Compiling

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

* Arch Linux ([community](https://archlinux.org/packages/extra/x86_64/check-sieve/) repository)

##### Snap

Installation of the latest edge snap from snapcraft can be done with the following command:
```
snap install --edge check-sieve
```

