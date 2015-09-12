#include <string.h>
#include <iostream>
#include <fstream>
#include "checksieve.h"
#include "sieve_driver.hh"
#include "AST.hh"

const char *usage_string  =
"Usage: check-sieve [options] file1 [file2 ...]                                 \n"
"                                                                               \n"
"Options:                                                                       \n"
"  --help                   Show this message                                   \n"
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

void traverse_syntax_tree(sieve::ASTNode *node, sieve::ASTTraceVisitor &visitor, int indent_level) {
    for (int i = 0; i < indent_level; i++) {
        std::cout << '\t';
    }
    
    node->accept(visitor);
    
    std::vector<sieve::ASTNode *> children = node->children();
    for (std::vector<sieve::ASTNode *>::iterator it = children.begin(); it != children.end(); ++it) {
        sieve::ASTNode *child = *it;
        traverse_syntax_tree(child, visitor, indent_level + 1);
    }
}

int main( int argc, char *argv[] ) {
    int result = 0;

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
        // Sieve file
        else {
            if (!file_exists(argv[i])) {
                std::cout << "Could not find file \"" << argv[i] << "\"." << std::endl;
                result = 2;
                break;
            }

            if ( !driver.parse_file(argv[i]) ) {
                if (driver.trace_tree) {
                    sieve::ASTTraceVisitor visitor = sieve::ASTTraceVisitor();
                    sieve::ASTSieve *sieve = driver.syntax_tree();
                    traverse_syntax_tree(sieve, visitor, 0);
                } else {
                    std::cout << "No errors found!" << std::endl;
                }
            } else {
                result = 1;
            }
        }
    }

    return result;
}
