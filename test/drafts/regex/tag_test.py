import sys
sys.path.append('./')

import unittest
import checksieve

class TestIndex(unittest.TestCase):

    def test_regex(self):
        sieve = '''
   require ["fileinto", "regex", "variables"];

   if header :regex "List-ID" "<(.*)@" {
       fileinto "lists.${1}"; stop;
   }

   # Imagine the header
   # Subject: [acme-users] [fwd] version 1.0 is out
   if header :regex "Subject" "^[(.*)] (.*)$" {
       # ${1} will hold "acme-users] [fwd"
       stop;
   }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_regex_no_require(self):
        sieve = '''
   require ["fileinto", "variables"];

   if header :regex "List-ID" "<(.*)@" {
       fileinto "lists.${1}"; stop;
   }

   # Imagine the header
   # Subject: [acme-users] [fwd] version 1.0 is out
   if header :regex "Subject" "^[(.*)] (.*)$" {
       # ${1} will hold "acme-users] [fwd"
       stop;
   }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_regex_2(self):
        sieve = '''
        require "regex";

   # Try to catch unsolicited email.
   if anyof (
     # if a message is not to me (with optional +detail),
     not address :regex ["to", "cc", "bcc"]
       "me(\\\\+.*)?@company\\\\.com",

     # or the subject is all uppercase (no lowercase)
     header :regex :comparator "i;octet" "subject"
       "^[^[:lower:]]+$" ) {

     discard;    # junk it
   }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_quoteregex(self):
        sieve = '''
        require ["regex", "variables"];
        set :quoteregex "b" "[a-zA-Z]*";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_quoteregex_no_require(self):
        sieve = '''
        require ["variables"];
        set :quoteregex "b" "[a-zA-Z]*";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
