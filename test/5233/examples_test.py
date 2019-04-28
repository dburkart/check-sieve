import unittest
import checksieve

class ExamplesTest(unittest.TestCase):
    
    def test_example1(self):
        sieve = '''
        require ["envelope", "subaddress", "fileinto"];

        # In this example the same user account receives mail for both
        # "ken@example.com" and "postmaster@example.com"

        # File all messages to postmaster into a single mailbox,
        # ignoring the :detail part.
        if envelope :user "to" "postmaster" {
            fileinto "inbox.postmaster";
            stop;
        }

        # File mailing list messages (subscribed as "ken+mta-filters").
        if envelope :detail "to" "mta-filters" {
            fileinto "inbox.ietf-mta-filters";
        }

        # Redirect all mail sent to "ken+foo".
        if envelope :detail "to" "foo" {
            redirect "ken@example.net";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()