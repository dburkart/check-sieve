import sys
sys.path.append('./')

import unittest
import checksieve

class TestCommands(unittest.TestCase):

    def test_vacation_no_args(self):
        sieve = '''
          require "vacation";
          vacation;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_vacation_bad_tag(self):
        sieve = '''
          require ["vacation", "copy"];
          vacation :copy "I'm out";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
