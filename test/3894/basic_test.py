import unittest
import checksieve

class TestBasic(unittest.TestCase):
    
    def test_copy_fileinto (self):
        sieve = '''
            require ["copy", "fileinto"];
            
            if header :matches "Subject" "*" {
                fileinto :copy "All Mail";
            }
            
            keep;
            stop;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_copy_redirect (self):
        sieve = '''
            require ["copy"];
            
            if header :matches "Subject" "*" {
                redirect :copy "foo@example.com";
            }
            
            keep;
            stop;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_copy_no_require (self):
        sieve = '''
            require ["fileinto"];
            
            if header :matches "Subject" "*" {
                fileinto :copy "All Mail";
            }
            
            keep;
            stop;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()