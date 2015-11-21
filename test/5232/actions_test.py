import sys
sys.path.append('./')

import unittest
import checksieve

class TestSetFlag(unittest.TestCase):
    
    def test_setflag(self):
        sieve = '''
        require ["imap4flags"];
        setflag "\\Seen";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_setflag_with_variable(self):
        sieve = '''
        require ["imap4flags", "fileinto"];
        if header :contains "from" "boss@frobnitzm.example.edu" {
            setflag "flagvar" "\\Flagged";
            fileinto :flags "${flagvar}" "INBOX.From Boss";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_setflag_no_args(self):
        sieve = '''
        require ["imap4flags"];
        setflag;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    

class TestAddFlag(unittest.TestCase):

    def test_addflag(self):
        sieve = '''
        require ["imap4flags"];
        addflag "\\Seen";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_addflag_with_var(self):
        sieve = '''
        require ["imap4flags"];
        addflag "flagvar" ["\\Deleted", "\\Answered"];
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_addflag_no_args(self):
        sieve = '''
        require ["imap4flags"];
        addflag;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()