import unittest
import checksieve

class TestBasic(unittest.TestCase):

    def test_reject(self):
        sieve = '''
            require ["reject"];
            reject "I don't like mail";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_reject_no_arg(self):
        sieve = '''
            require ["reject"];
            reject;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_reject_no_require(self):
        sieve = '''
            reject "I don't like mail";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_ereject(self):
        sieve = '''
            require ["ereject"];
            ereject "I don't like mail";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_ereject_no_arg(self):
        sieve = '''
            require ["ereject"];
            ereject;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_ereject_no_require(self):
        sieve = '''
            ereject "I don't like mail";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_with_multiline_reason(self):
        sieve = '''
      require ["reject"];

      if size :over 100K {
          reject text:
Your message is too big.  If you want to send me a big attachment,
put it on a public web site and send me a URL.
.
          ;
      }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()