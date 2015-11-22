import sys
sys.path.append('./')

import unittest
import checksieve

class TestIf(unittest.TestCase):

    def test_simple_if(self):
        sieve = 'if header :contains :comparator "i;octet" "Subject" "MAKE MONEY FAST" { discard; }'
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_bogus_test(self):
        sieve = 'if bogus :contains :comparator "i;octet" "Subject" "MAKE MONEY FAST" { discard; }'
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

    def test_if_elsif_elsif_else(self):
        sieve = '''if header :matches ["Subject"] ["MAKE MONEY FAST"]
        {
            discard;
        }
        elsif header :matches "Subject" "business opportunity" {
            discard;
        }
        elsif header :matches "Subject" "NOT A VIRUS" {
            discard;
        }
        else {
            keep;
            stop;
        }'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_test_list(self):
        sieve = '''
            if allof
            (
                header :contains "List-Id" "<lunch@group.website.com>",
                header :matches "Subject" [ "*Closure*", "*closure*" ]
            )
            {
                keep;
                stop;
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_empty_block(self):
        sieve = '''
            if header :matches "Subject" "NOT A VIRUS" {

            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
