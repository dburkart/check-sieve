import unittest
import checksieve

class TestSpamtest(unittest.TestCase):
    
    def test_without_percent(self):
        sieve = '''
        require ["spamtest", "fileinto", "relational", "comparator-
                 i;ascii-numeric"];
        if spamtest :value "eq" :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.unclassified";
        }
        elsif spamtest :value "ge" :comparator "i;ascii-numeric" "3"
        {
            fileinto "INBOX.spam-trap";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_with_percent(self):
        sieve = '''
        require ["spamtestplus", "fileinto", "relational",
                 "comparator-i;ascii-numeric"];

        if spamtest :value "eq"
                    :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.unclassified";
        }
        elsif spamtest :percent :value "eq"
                        :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.not-spam";
        }
        elsif spamtest :percent :value "lt"
                        :comparator "i;ascii-numeric" "37"
        {
            fileinto "INBOX.spam-trap";
        }
        else
        {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_with_count(self):
        sieve = '''
        require ["spamtestplus", "fileinto", "relational",
                 "comparator-i;ascii-numeric"];
        if spamtest :percent :count "eq"
                    :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.unclassified";
        }
        elsif spamtest :percent :value "eq"
                        :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.not-spam";
        }
        elsif spamtest :percent :value "lt"
                        :comparator "i;ascii-numeric" "37"
        {
            fileinto "INBOX.spam-trap";
        }
        else
        {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()