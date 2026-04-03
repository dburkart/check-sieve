import unittest
import checksieve

class TestCommands(unittest.TestCase):

    def test_vacation_seconds_missing_require(self):
        sieve = '''
          require "vacation";
          vacation :seconds 3600 "I'm out";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_vacation_days_and_seconds_exclusive(self):
        sieve = '''
          require "vacation-seconds";
          vacation :days 7 :seconds 3600 "I'm out";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
