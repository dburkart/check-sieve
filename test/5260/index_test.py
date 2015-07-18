import sys
sys.path.append('./')

import unittest
import checksieve

class TestIndex(unittest.TestCase):

    def test_index(self):
        sieve = '''
     # Implement the Internet-Draft cutoff date check assuming the
     # second Received: field specifies when the message first
     # entered the local email infrastructure.
     require ["date", "relational", "index"];
     if date :value "gt" :index 2 :zone "-0500" "received"
             "iso8601" "2007-02-26T09:00:00-05:00"
     { redirect "aftercutoff@example.org"; }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()