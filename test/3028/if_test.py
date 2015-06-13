import sys
sys.path.append('./')

import unittest
import checksieve

class TestIf(unittest.TestCase):
    
    def test_simple_if(self):
        sieve = 'if header :contains :comparitor "i;octet" "Subject" "MAKE MONEY FAST" { discard; }'
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_bogus_test(self):
        sieve = 'if bogus :contains :comparitor "i;octet" "Subject" "MAKE MONEY FAST" { discard; }'
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_elsif(self):
        sieve = 'if header :matches "Subject" "MAKE MONEY FAST" { discard; }\nelsif header :matches "Subject" "business opportunity" { discard; }'
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_elsif_multiple(self):
        sieve = '''if header :matches "Subject" "MAKE MONEY FAST" 
        { 
            discard; 
        }
        elsif header :matches "Subject" "business opportunity" { 
            discard; 
        }
        elsif header :matches "To" "someone@example.com" {
            keep;
            stop;
        }'''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_else(self):
        sieve = '''
        if header :matches "Subject" "MAKE MONEY FAST" {
            discard;
        } else {
            keep;
            stop;
        }'''
        self.assertFalse(checksieve.parse_string(sieve, False));
    
    def test_if_elsif_else(self):
        sieve = '''if header :matches ["Subject"] ["MAKE MONEY FAST"] 
        { 
            discard; 
        }
        elsif header :matches "Subject" "business opportunity" { 
            discard; 
        }
        else {
            keep;
            stop;
        }'''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()