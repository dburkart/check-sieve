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


if __name__ == '__main__':
    unittest.main()
