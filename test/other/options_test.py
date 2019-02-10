import sys
sys.path.append('./')

import unittest
import checksieve

class OptionsTest(unittest.TestCase):

    def test_max_list_length(self):
        sieve = '''
            require ["include", "variables"];

            global "test";
        '''
        self.assertFalse(checksieve.parse_string_with_options(sieve, {"string_list_max_length": 2}))

    def test_max_list_length(self):
        sieve = '''
            require ["include", "variables", "imap4flags"];

            global "test";
        '''
        self.assertTrue(checksieve.parse_string_with_options(sieve, {"string_list_max_length": 2}))

if __name__ == '__main__':
    unittest.main()
