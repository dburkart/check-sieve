import sys
sys.path.append('./')

import unittest
import checksieve

class ReturnTest(unittest.TestCase):

    def test_return(self):
        sieve = '''
            require ["include"];

            return;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_return_extra_arg(self):
        sieve = '''
            require ["include"];

            return "foo";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
