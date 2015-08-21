#include <string.h>
#include <iostream>
#include <fstream>
#include "sieve_driver.hh"

const char *usage_string  =
"Usage: check-sieve [options] file1 [file2 ...]                                 \n"
"                                                                               \n"
"Options:                                                                       \n"
"  --help                   Show this message                                   \n"
"  --trace-parser           Trace the operation of the parser                   \n"
"  --trace-scanner          Trace the operation of the scanner                  \n"
"  --version                Print out version information                       \n";

void print_version() {
    std::cout << "check-sieve, version " << sieve::version() << " (" << PLATFORM << ")" << std::endl;
}

void print_help() {
    std::cout << usage_string << std::endl;
}

bool file_exists(const char *filename) {
    std::ifstream file(filename);
    return (bool)file;
}

int main( int argc, char *argv[] ) {
    int result = 0;

    if (argc == 1) {
        print_help();
        return 1;
    }

    sieve::sieve_driver driver;
    for (int i = 1; i < argc; ++i) {
        // Long argument
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (strcmp(argv[i], "--trace-scanner") == 0) {
                driver.trace_scanning = true;
                continue;
            }

            if (strcmp(argv[i], "--trace-parser") == 0) {
                driver.trace_parsing = true;
                continue;
            }

            if (strcmp(argv[i], "--help") == 0) {
                print_help();
                return 0;
            }

            if (strcmp(argv[i], "--version") == 0) {
                print_version();
                return 0;
            }

            std::cout << "Unrecognized argument \"" << argv[i] << "\"" << std::endl;
            return 1;
        }
        // Sieve file
        else {
            if (!file_exists(argv[i])) {
                std::cout << "Could not find file \"" << argv[i] << "\"." << std::endl;
                result = 2;
                break;
            }

            if ( !driver.parse_file(argv[i]) ) {
                std::cout << "No errors found!" << std::endl;
            } else {
                result = 1;
            }
        }
    }

    return result;
}
