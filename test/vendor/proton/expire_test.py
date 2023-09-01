import unittest
import checksieve


class TestProtonExpire(unittest.TestCase):
    def test_expire(self):
        sieve = '''
            require "vnd.proton.expire";
            # permanently delete all incoming and outgoing emails after 10 days
            expire "day" "10";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_expire_no_require(self):
        sieve = '''
            # permanently delete all incoming and outgoing emails after 10 days
            expire "day" "10";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_hasexpiration(self):
        sieve = '''
            require "vnd.proton.expire";
            
            if hasexpiration {
                stop;
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_hasexpiration_no_require(self):
        sieve = '''
            if hasexpiration {
                stop;
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_expiration(self):
        sieve = '''
            require "vnd.proton.expire";
            require "comparator-i;ascii-numeric";
            
            if expiration :comparator "i;ascii-numeric" "ge" "day" "5"  {
                stop;
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
