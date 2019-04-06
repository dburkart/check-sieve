import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
           require "vacation";
           if header :contains "subject" "cyrus" {
               vacation "I'm out -- send mail to cyrus-bugs";
           } else {
               vacation "I'm out -- call me at +1 304 555 0123";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
           require ["vacation", "variables"];
           if header :matches "subject" "*" {
               vacation :subject "Automatic response to: ${1}"
                        "I'm away -- send mail to foo in my absence";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = '''
           require "vacation";
           vacation :days 23 :addresses ["tjs@example.edu",
                                         "ts4z@landru.example.edu"]
           "I'm away until October 19.
           If it's an emergency, call 911, I guess." ;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_4(self):
        sieve = '''
           require "vacation";
           if header :contains "from" "boss@example.edu" {
               redirect "pleeb@isp.example.org";
           } else {
               vacation "Sorry, I'm away, I'll read your
           message when I get around to it.";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_5(self):
        sieve = '''
           require "vacation";
           if header :contains ["accept-language", "content-language"] "en"
           {
               vacation "I am away this week.";
           } else {
               vacation "Estoy ausente esta semana.";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_mime(self):
        sieve = '''
require "vacation";
vacation :mime text:
Content-Type: multipart/alternative; boundary=foo

--foo

I'm at the beach relaxing.  Mmmm, surf...

--foo
Content-Type: text/html; charset=us-ascii

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN"
"http://www.w3.org/TR/REC-html40/strict.dtd">
<HTML><HEAD><TITLE>How to relax</TITLE>
<BASE HREF="http://home.example.com/pictures/"></HEAD>
<BODY><P>I'm at the <A HREF="beach.gif">beach</A> relaxing.
Mmmm, <A HREF="ocean.gif">surf</A>...
</BODY></HTML>

--foo--
.
;
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
