import sys
sys.path.append('./')

import unittest
import checksieve

class TestVariables(unittest.TestCase):
    
    def test_set(self):
        sieve = '''
        require "variables";
        set "honorific" "Mr";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
        
    def test_mod_length(self):
        sieve = '''
        require "variables";
        set :length "b" "${a}";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_test_string(self):
        sieve = '''
        require "variables";
        set "state" "${state} pending";
        if string :matches " ${state} " "* pending *" {
          # the above test always succeeds
          stop;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()