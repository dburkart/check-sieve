import unittest
import checksieve

class TestMetadata(unittest.TestCase):

    def test_metadata(self):
        sieve = '''
require ["mboxmetadata", "vacation"];

if metadata :is "INBOX"
    "/private/vendor/vendor.isode/auto-replies" "on" {

    vacation text:
I'm away on holidays till March 2009.
Expect a delay.
.
;

}
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_metadataexists(self):
        sieve = '''
        require ["mboxmetadata", "vacation"];

        if metadataexists "INBOX"
            "/private/vendor/vendor.isode/auto-replies" {

            stop;

        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()