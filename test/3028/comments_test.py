import sys
sys.path.append('./')

import unittest
import checksieve

class TestComments(unittest.TestCase):
    
    def test_single_line(self):
        sieve = '# This is a comment'
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_single_line_with_code(self):
        sieve = 'keep; # This is a comment'
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()