import sys
sys.path.append('./')

import unittest
import checksieve

class TestCommands(unittest.TestCase):
    
    def test_discard(self):
        sieve = '''
        discard;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_discard_extra_arg(self):
        sieve = '''
        discard "foo";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_redirect(self):
        sieve = '''
        redirect "foo@example.org";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_redirect_no_args(self):
        sieve = '''
        redirect;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()