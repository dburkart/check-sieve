import sys
sys.path.append('./')

import unittest
import checksieve

class TestBody(unittest.TestCase):

    def test_body(self):
        sieve = '''
            require ["body"];

            if body :raw :contains "MAKE MONEY FAST" {
                discard;
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_body_no_require(self):
        sieve = '''
            if body :raw :contains "MAKE MONEY FAST" {
                discard;
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
