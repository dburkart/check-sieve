import sys
sys.path.append('./')

import unittest
import checksieve

class TestMiscellany(unittest.TestCase):
    
    def test_fileinto_no_require(self):
        sieve = 'fileinto "Inbox";'
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_fileinto(self):
        sieve = '''
        require "fileinto";
        fileinto "Inbox";'''
        self.assertFalse(checksieve.parse_string(sieve, False))
    

if __name__ == '__main__':
    unittest.main()