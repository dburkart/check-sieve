import sys
sys.path.append('./')

import unittest
import checksieve

class TestTests(unittest.TestCase):

    def test_address(self):
        sieve = '''
           require ["mime", "fileinto"];

           if address :mime :is :all "content-from" "tim@example.com"
           {
               fileinto "INBOX.part-from-tim";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_exists(self):
        sieve = '''
           require ["mime", "fileinto"];

           if exists :mime :anychild "content-md5"
           {
               fileinto "INBOX.md5";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
