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

    def test_multiple_is(self):
        sieve = '''
        if header :is "X-Phabricator-Mail-Tags" :is "<audit-commit>"
        {
            discard;
            stop;
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_single_test_parens_01(self):
        sieve = '''
        if anyof (not exists ["From", "Date"]) {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_single_test_parens_02(self):
        sieve = '''
        if (address :is true) {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_single_test_parens_03(self):
        sieve = '''
        if anyof (not true) {
          keep;
        } else {
          discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
