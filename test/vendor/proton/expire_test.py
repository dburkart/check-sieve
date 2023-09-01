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


if __name__ == '__main__':
    unittest.main()
