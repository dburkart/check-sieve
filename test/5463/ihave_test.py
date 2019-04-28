import unittest
import checksieve

class TestIHave(unittest.TestCase):

    def test_ihave_index(self):
        sieve = '''
        require "ihave";

        addheader "X-Hello" "World";
        if ihave "index" {
            deleteheader :index 1 "X-Hello";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))



if __name__ == '__main__':
    unittest.main()
