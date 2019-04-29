import unittest
import checksieve

class TestEnvironment(unittest.TestCase):

    def test_environment_name(self):
        sieve = '''
        require ["environment"];

        if environment :is "name" "check-sieve" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_domain(self):
        sieve = '''
        require ["environment"];

        if environment :is "domain" ["google.com", "icloud.com"] {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_host(self):
        sieve = '''
        require ["environment"];

        if environment :is "host" "moo" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_location(self):
        sieve = '''
        require ["environment"];

        if environment :is "location" "foo" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_phase(self):
        sieve = '''
        require ["environment"];

        if environment :is "phase" "1" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_remote_host(self):
        sieve = '''
        require ["environment"];

        if environment :is "remote-host" "moo" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_environment_remote_ip(self):
        sieve = '''
        require ["environment"];

        if environment :is "remote-ip" "127.0.0.1" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_environment_version(self):
        sieve = '''
        require ["environment"];

        if environment :is "version" "1.2" {
            discard;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_environment_bogus(self):
        sieve = '''
        require ["environment"];

        if environment :is "bogus" "check-sieve" {
            discard;
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
