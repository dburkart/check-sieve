import unittest
import checksieve

class TestTests(unittest.TestCase):
    
    def test_string(self):
        sieve = '''
            require ["variables"];
            if string :matches " ${state} " "* pending *" {
              # the above test always succeeds
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_string_no_require(self):
        sieve = '''
            if string :matches " ${state} " "* pending *" {
              # the above test always succeeds
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()