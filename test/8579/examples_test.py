import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
        require "imapsieve";
        require "special-use";
        require "environment";
        require "variables";

        if environment :contains "imap.mailbox" "*" {
            set "mailbox" "${1}";
        }

        if allof(
            environment "imap.cause" "COPY",
            specialuse_exists "${mailbox}" "\\Junk") {
            redirect "spam-report@example.org";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
        require "fileinto";
        require "special-use";

        fileinto :specialuse "\\Junk" "Spam";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = '''
        require "fileinto";
        require "special-use";
        require "mailbox";

        fileinto :specialuse "\\Junk" :create "Spam";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_4(self):
        sieve = '''
        require "special-use";

        if (address :specialuse "foo") {
            discard;
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
