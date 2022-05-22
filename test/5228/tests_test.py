import unittest
import checksieve

class TestForTestList(unittest.TestCase):
    
    def test_not_01(self):
        sieve = '''
            if not (address :is "Foo") {
                discard;
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()