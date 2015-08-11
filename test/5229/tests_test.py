import sys
sys.path.append('./')

import unittest
import checksieve

class TestTests(unittest.TestCase):
    
    def test_hasflag(self):
        sieve = '''
        if hasflag :contains "MyVar" "Junk" {
            discard;
            stop;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()