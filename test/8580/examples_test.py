import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
            require ["vacation", "fcc", "mailbox", "imap4flags"];

           vacation :days 7
                    :from "hemingway@example.com" "Gone Fishin'"
                    :fcc "INBOX.Sent";
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
        
   require ["enotify", "fcc"];

   notify :fcc "INBOX.Sent"
          :message "You got mail!"
          "mailto:ken@example.com";

        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = '''
        
        require ["enotify", "fcc"];

   if notify_method_capability "xmpp:" "fcc" "yes" {
       notify :fcc "INBOX.Sent"
              :message "You got mail"
              "xmpp:ken@example.com?message;subject=SIEVE";
   } else {
       notify :fcc "INBOX.Sent"
              :message "You got mail!"
              "mailto:ken@example.com";
   }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_no_reject(self):
        sieve = '''
      require ["reject", "fcc"];

      if size :over 100K {
          reject :fcc "foo" text:
Your message is too big.  If you want to send me a big attachment,
put it on a public web site and send me a URL.
.
          ;
      }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))