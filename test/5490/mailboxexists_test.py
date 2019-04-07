import unittest
import checksieve

class TestMailboxexists(unittest.TestCase):

    def test_mailboxexists(self):
        sieve = '''
        require ["fileinto", "reject", "mailbox"];
        if mailboxexists "Partners" {
           fileinto "Partners";
        } else {
           reject "This message was not accepted by the Mailstore";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()