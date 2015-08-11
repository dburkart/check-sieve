import sys
sys.path.append('./')

import unittest
import checksieve

class TestTests(unittest.TestCase):
    
    def test_hasflag(self):
        sieve = '''
        require ["imap4flags"];
        if hasflag :contains "MyVar" "Junk" {
            discard;
            stop;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_hasflag_no_require(self):
        sieve = '''
        if hasflag :contains "MyVar" "Junk" {
            discard;
            stop;
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()