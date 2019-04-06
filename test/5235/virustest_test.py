import unittest
import checksieve

class TestVirustest(unittest.TestCase):
    
    def test_virustest(self):
        sieve = '''
        require ["virustest", "fileinto", "relational", "comparator-
                i;ascii-numeric"];

        if virustest :value "eq" :comparator "i;ascii-numeric" "0"
        {
            fileinto "INBOX.unclassified";
        }
        if virustest :value "eq" :comparator "i;ascii-numeric" "4"
        {
            fileinto "INBOX.quarantine";
        }
        elsif virustest :value "eq" :comparator "i;ascii-numeric" "5"
        {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()