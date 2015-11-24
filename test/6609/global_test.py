import sys
sys.path.append('./')

import unittest
import checksieve

class GlobalTest(unittest.TestCase):

    def test_global(self):
        sieve = '''
            require ["include", "variables"];

            global "test";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_global_no_require(self):
        sieve = '''
            global "test";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_global_no_include_require(self):
        sieve = '''
            require ["variables"];

            global "test";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_global_no_variables_require(self):
        sieve = '''
            require ["include"];

            global "test";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_global_list(self):
        sieve = '''
            require ["include", "variables"];

            global ["foo", "bar", "baz"];
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
