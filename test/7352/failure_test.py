import unittest
import checksieve

class TestFailures(unittest.TestCase):

    def test_failure_1(self):
        sieve = '''
		   require ["duplicate", "fileinto", "mailbox"];

           if duplicate :header "foo" :uniqueid "bar" {
             fileinto :create "Trash/Duplicate";
           }
		'''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_failure_2(self):
        sieve = '''
		   require ["duplicate", "fileinto", "mailbox"];

           if duplicate :index 2 {
             fileinto :create "Trash/Duplicate";
           }
		'''
        self.assertTrue(checksieve.parse_string(sieve, True))



if __name__ == '__main__':
    unittest.main()
