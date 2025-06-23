import unittest
import checksieve

class RegressionTest(unittest.TestCase):

    def test_case_sensitivity(self):
        sieve = '''
        require [ "envelope", "subaddress", "fileinto", "mailbox" ];

        if anyof ( envelope :user "To" "yt",
              address :user ["To", "CC"] "yt" ) { fileinto :create "YouTube"; stop; }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))