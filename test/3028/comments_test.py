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
    
    def test_multi_line(self):
        sieve = '''
            /* This is the first line
               This is the second */
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_multi_line_2(self):
        sieve = '''
        if exists "In-Reply-To" {
            /* Single-line-multi-line-comment */
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_multi_line_3(self):
        sieve = '''
        if exists "In-Reply-To" {
            /* Multi-line comment
            with a * in it */
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()