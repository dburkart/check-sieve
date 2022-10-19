import unittest
import checksieve

class TestRelational(unittest.TestCase):

    def test_count(self):
        sieve = '''
          require ["relational", "fileinto", "comparator-i;ascii-numeric"];
          if address :count "gt" :comparator "i;ascii-numeric"
                     ["to"] ["5"]
          {
             # everything with more than 5 recipients in the "to" field
             # is considered SPAM
             fileinto "SPAM";
          }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_count_no_require(self):
        sieve = '''
          require ["fileinto"];
          if address :count "gt" :comparator "i;ascii-numeric"
                     ["to"] ["5"]
          {
             # everything with more than 5 recipients in the "to" field
             # is considered SPAM
             fileinto "SPAM";
          }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_value(self):
        sieve = '''
           require ["relational", "fileinto", "comparator-i;ascii-casemap"];
           if address :value "gt" :all :comparator "i;ascii-casemap"
                 ["from"] ["M"]
          {
             fileinto "From N-Z";
          }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_header_test_value(self):
        sieve = '''
          require ["relational", "comparator-i;ascii-numeric", "fileinto"];
          if header :value "lt" :comparator "i;ascii-numeric"
             ["x-priority"] ["3"]
          {
             fileinto "Priority";
          }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_value_no_require(self):
        sieve = '''
           require ["fileinto"];
           if address :value "gt" :all :comparator "i;ascii-casemap"
                 ["from"] ["M"]
          {
             fileinto "From N-Z";
          }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    # TODO: Re-enable this test when it passes
    # def test_value_no_comparator_require(self):
    #     sieve = '''
    #        require ["fileinto", "relational"];
    #        if address :value "gt" :all :comparator "i;ascii-numeric"
    #              ["from"] ["M"]
    #       {
    #          fileinto "From N-Z";
    #       }
    #     '''
    #     self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
