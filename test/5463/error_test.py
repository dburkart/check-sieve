import unittest
import checksieve

class TestError(unittest.TestCase):

    def test_error(self):
        sieve = '''
        require "ihave";

        addheader "X-Hello" "World";
        if ihave "index" {
            deleteheader :index 1 "X-Hello";
        } else {
            error "Abort!";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_error_no_message(self):
        sieve = '''
        require "ihave";

        addheader "X-Hello" "World";
        if ihave "index" {
            deleteheader :index 1 "X-Hello";
        } else {
            error;
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()