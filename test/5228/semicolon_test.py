import unittest
import checksieve

class SemiColonTest(unittest.TestCase):
    
    def test_dangling_semicolon(self):
        sieve = '''
       require ["fileinto"];
if header :matches "Subject" "*Some Subject:*"  {                  ; fileinto "Folder"; stop; }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()