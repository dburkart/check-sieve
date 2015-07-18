import sys
sys.path.append('./')

import unittest
import checksieve

class TestCurrentDate(unittest.TestCase):

    def test_current_date(self):
        sieve = '''
     require ["date", "relational"];
     if anyof(currentdate :is "weekday" "0",
              currentdate :is "weekday" "6",
              currentdate :value "lt" "hour" "09",
              currentdate :value "ge" "hour" "17")
     { redirect "pager@example.com"; }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()