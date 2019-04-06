import unittest
import checksieve

class TestAddHeader(unittest.TestCase):

    def test_addheader_correct(self):
        sieve = '''
        if not header :contains "X-Sieve-Filtered"
                ["<kim@job.example.com>", "<kim@home.example.com>"]
        {
                addheader "X-Sieve-Filtered" "<kim@job.example.com>";
                redirect "kim@home.example.com";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_addheader_correct_lasttag(self):
        sieve = '''
        if not header :contains "X-Sieve-Filtered"
                ["<kim@job.example.com>", "<kim@home.example.com>"]
        {
                addheader :last "X-Sieve-Filtered" "<kim@job.example.com>";
                redirect "kim@home.example.com";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_addheader_incorrect_tag(self):
        sieve = '''
        if not header :contains "X-Sieve-Filtered"
                ["<kim@job.example.com>", "<kim@home.example.com>"]
        {
                addheader :is "X-Sieve-Filtered" "<kim@job.example.com>";
                redirect "kim@home.example.com";
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_addheader_incorrect_number_of_args(self):
        sieve = '''
        if not header :contains "X-Sieve-Filtered"
                ["<kim@job.example.com>", "<kim@home.example.com>"]
        {
                addheader :is "X-Sieve-Filtered";
                redirect "kim@home.example.com";
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_addheader_incorrect_type(self):
        sieve = '''
        if not header :contains "X-Sieve-Filtered"
                ["<kim@job.example.com>", "<kim@home.example.com>"]
        {
                addheader :last :is "X-Sieve-Filtered";
                redirect "kim@home.example.com";
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_add_and_delete_1(self):
        sieve = '''
        require ["index"];
        addheader "X-Hello" "World";
        deleteheader :index 1 "X-Hello";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_delete_header(self):
        sieve = '''
        require ["index"];
        deleteheader :index 1 :last "X-Hello";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_delete_header_incorrect_last(self):
        sieve = '''
        require "index";
        deleteheader :index 1 "X-Hello" :last;
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
