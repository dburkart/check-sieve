#include <iostream>
#include <fstream>
#include "sieve_driver.hh"

bool file_exists(const char *filename) {
    std::ifstream file(filename);
    return file;
}

int main( int argc, char *argv[] ) {
    int result = 0;
    
    sieve_driver driver;
    for (int i = 1; i < argc; ++i) {
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
    
    return result;
}
