import unittest
import checksieve

class TestDate(unittest.TestCase):

    def test_date_pass(self):
        sieve = '''
     require ["date", "relational", "fileinto"];
     if anyof(date :is "received" "weekday" "0",
              date :is "received" "weekday" "6")
     { fileinto "weekend"; }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_date_no_require(self):
        sieve = '''
     require ["relational", "fileinto"];
     if anyof(date :is "received" "weekday" "0",
              date :is "received" "weekday" "6")
     { fileinto "weekend"; }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_date_2(self):
        sieve = '''
             require ["date", "relational", "fileinto"];
     if allof(header :is "from" "boss@example.com",
              date :value "ge" :originalzone "date" "hour" "09",
              date :value "lt" :originalzone "date" "hour" "17")
     { fileinto "urgent"; }
     '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()