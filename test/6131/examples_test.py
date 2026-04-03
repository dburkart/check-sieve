import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_seconds_basic(self):
        sieve = '''
          require "vacation-seconds";
          vacation :seconds 3600 "I'm out for a bit.";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_seconds_with_vacation_require(self):
        sieve = '''
          require ["vacation", "vacation-seconds"];
          vacation :seconds 86400 "I'm away for the day.";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_seconds_with_subject(self):
        sieve = '''
          require "vacation-seconds";
          vacation :seconds 7200 :subject "Auto-reply" "I'm away.";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_seconds_with_addresses(self):
        sieve = '''
          require "vacation-seconds";
          vacation :seconds 3600
                   :addresses ["user@example.com", "alias@example.com"]
                   "I'm unavailable.";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_seconds_zero(self):
        sieve = '''
          require "vacation-seconds";
          vacation :seconds 0 "Back soon.";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
