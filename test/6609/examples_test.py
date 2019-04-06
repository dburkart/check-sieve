import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
            require ["include"];

            include :personal "always_allow";
            include :global "spam_tests";
            include :personal "spam_tests";
            include :personal "mailing_lists";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
            require ["variables", "include"];

            set "global.i_am_on_vacation" "1";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_return_command_no_require(self):
        sieve = '''
            return;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_return_command(self):
        sieve = '''
            require ["include"];
            return;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_include_no_arguments(self):
        sieve = '''
            require ["include"];
            include;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
