import unittest
import checksieve

class TestCreateTag(unittest.TestCase):

    def test_create_tag(self):
        sieve = '''
        require ["fileinto", "reject", "mailbox"];
        if mailboxexists "Partners" {
           fileinto :create "Partners";
        } else {
           reject "This message was not accepted by the Mailstore";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()