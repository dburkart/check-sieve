import unittest
import checksieve

class TestFileinto(unittest.TestCase):
    
    def test_fileinto_with_flags(self):
        sieve = '''
        require ["fileinto", "imap4flags"];
        fileinto :flags "\\\\Seen" "Mailbox";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_fileinto_with_flags_list(self):
        sieve = '''
        require ["fileinto", "imap4flags"];
        fileinto :flags ["\\\\Seen", "\\\\Deleted"] "Mailbox";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_fileinto_with_flags_without_arguments(self):
        sieve = '''
        require ["fileinto", "imap4flags"];
        fileinto :flags "Mailbox";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_fileinto_with_flag_and_copy(self):
        sieve = '''
        require ["fileinto", "imap4flags", "copy"];
        fileinto :flags "\\\\Seen" :copy "Mailbox";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_fileinto_with_copy_and_flag(self):
        sieve = '''
        require ["fileinto", "imap4flags", "copy"];
        fileinto :copy :flags "\\\\Seen" "Mailbox";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_fileinto_with_flag_without_arguments_and_copy(self):
        sieve = '''
        require ["fileinto", "imap4flags", "copy"];
        fileinto :flags :copy "Mailbox";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    
    def test_fileinto_with_copy_and_flag_without_arguments(self):
        sieve = '''
        require ["fileinto", "imap4flags", "copy"];
        fileinto :copy :flags "Mailbox";
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))
    


if __name__ == '__main__':
    unittest.main()