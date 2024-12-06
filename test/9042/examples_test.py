import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
        require "fileinto";
        require "mailboxid";

        if header :contains ["from"] "coyote" {
            fileinto :mailboxid "F6352ae03-b7f5-463c-896f-d8b48ee3"
                     "INBOX.harassment";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
        require "fileinto";
        require "mailboxid";
        require "mailbox";

        fileinto :mailboxid "Fnosuch"
                 :create
                 "INBOX.no-such-folder";
                    # creates INBOX.no-such-folder, but it doesn't
                    # get the "Fnosuch" mailboxid.
            '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = '''
        require "fileinto";
        require "mailboxid";

        if header :contains ["from"] "coyote" {
            if mailboxidexists "F6352ae03-b7f5-463c-896f-d8b48ee3" {
                fileinto :mailboxid "F6352ae03-b7f5-463c-896f-d8b48ee3"
                                    "INBOX.name.will.not.be.used";
            } else {
                fileinto "INBOX.harassment";
            }
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_specialuse_interaction(self):
        sieve = '''
        require "fileinto";
        require "special-use";
        require "mailboxid";

        fileinto :mailboxid "blah" :specialuse "\\Junk" "Spam";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
