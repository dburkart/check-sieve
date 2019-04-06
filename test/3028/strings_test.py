import unittest
import checksieve

class TestStrings(unittest.TestCase):
    
    def test_multiline_strings(self):
        sieve = '''
            require "vacation";
            vacation :mime
            "Content-Type: multipart/alternative; boundary=foo

            --foo

            I'm at the beach relaxing.  Mmmm, surf...

            --foo
            Content-Type: text/html; charset=us-ascii

            <!DOCTYPE HTML PUBLIC \\"-//W3C//DTD HTML 4.0//EN\\"
            \\"http://www.w3.org/TR/REC-html40/strict.dtd\\">
            <HTML><HEAD><TITLE>How to relax</TITLE>
            <BASE HREF=\\"http://home.example.com/pictures/\\"></HEAD>
            <BODY><P>I'm at the <A HREF=\\"beach.gif\\">beach</A> relaxing.
            Mmmm, <A HREF=\\"ocean.gif\\">surf</A>...
            </BODY></HTML>

            --foo--
            ";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
