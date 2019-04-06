import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
        require ["body", "fileinto"];

        # Save any message with any text MIME part that contains the
        # words "missile" or "coordinates" in the "secrets" folder.

        if body :content "text" :contains ["missile", "coordinates"] {
                fileinto "secrets";
        }

        # Save any message with an audio/mp3 MIME part in
        # the "jukebox" folder.

        if body :content "audio/mp3" :contains "" {
                fileinto "jukebox";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
