#include <string.h>
#include <iostream>
#include <fstream>

#include "AST.hh"
#include "checksieve.h"
#include "diagnostic.hh"
#include "sieve_driver.hh"

const char *usage_string  =
"Usage: check-sieve [options] file1 [file2 ...]                                 \n"
"                                                                               \n"
"Options:                                                                       \n"
"  -h, --help               Show this message                                   \n"
"  --trace-parser           Trace the operation of the parser                   \n"
"  --trace-scanner          Trace the operation of the scanner                  \n"
"  --trace-tree             Trace the abstract-syntax-tree                      \n"
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
    sieve::Diagnostic diag;

    if (argc == 1) {
        print_help();
        return 1;
    }

    sieve::driver driver;
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

            if (strcmp(argv[i], "--trace-tree") == 0) {
                driver.trace_tree = true;
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
        // Short argument
        else if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-h") == 0) {
                print_help();
                return 0;
            }
        }
        // Sieve file
        else {
            if (!file_exists(argv[i])) {
                std::cout << "Could not find file \"" << argv[i] << "\"." << std::endl;
                result = 2;
                break;
            }

            sieve::parse_result parse_res = driver.parse_file( argv[i] );

            if ( !parse_res.status ) {
                if (driver.trace_tree) {
                    sieve::ASTTraceVisitor visitor = sieve::ASTTraceVisitor();
                    visitor.walk(driver.syntax_tree());
                } else {
                    std::cout << "No errors found!" << std::endl;
                }
            } else {
                std::ifstream fin( argv[i] );
                std::cerr << diag.describe(parse_res, fin);
                result = 1;
            }
        }
    }

    return result;
}
