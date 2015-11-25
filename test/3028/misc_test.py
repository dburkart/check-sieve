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

    def test_unnecessary_semicolon(self):
        sieve = 'if header :matches "Subject" "*" { keep; stop; };'
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_multiline_string(self):
        sieve = '''
        require "fileinto";
        fileinto text:
            Inbox
.
;'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_multiline_string_with_comment(self):
        sieve = '''
        require "fileinto";
        fileinto text: #inline comment
            Inbox
.
;'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_multiple_requires(self):
        sieve = '''
        require "fileinto";
        require "copy";
        
        if header :matches "Subject" "*" {
            fileinto :copy "All Mail";
        }
        
        keep;
        stop;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_stop_too_many_args(self):
        sieve = '''
        stop "foo";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
