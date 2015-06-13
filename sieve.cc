#include <iostream>
#include "sieve_driver.hh"


int main( int argc, char *argv[] ) {
    int result = 0;
    
    sieve_driver driver;
    for (int i = 1; i < argc; ++i) {
        if ( !driver.parse_file(argv[i]) ) {
            std::cout << "No errors found!" << std::endl;
        } else {
            result = 1;
        }
    }
    
    return result;
}
