import sys
sys.path.append('./')

import unittest
import checksieve

class TestKeep(unittest.TestCase):
    
    def test_hasflag(self):
        sieve = '''
        require ["imap4flags"];
        if hasflag :contains "MyVar" "Junk" {
            keep :flags "\\\\Junk";
            stop;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    


if __name__ == '__main__':
    unittest.main()