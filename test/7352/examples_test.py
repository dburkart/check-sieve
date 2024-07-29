import unittest
import checksieve

class TestExamples(unittest.TestCase):

    def test_example_1(self):
        sieve = '''
		   require ["duplicate", "fileinto", "mailbox"];

           if duplicate {
             fileinto :create "Trash/Duplicate";
           }
		'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = '''
            require ["duplicate", "variables", "imap4flags", "fileinto"];
            
            if header :matches "subject" "ALERT: *" {
             if duplicate :seconds 60 :uniqueid "${1}" {
               setflag "\\seen";
             }
             fileinto "Alerts";
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = '''
            require ["variables", "envelope", "enotify", "duplicate"];

           if envelope :matches "from" "*" { set "sender" "${1}"; }
           if header :matches "subject" "*" { set "subject" "${1}"; }
        
           if not duplicate :seconds 1800 :uniqueid "${sender}"
           {
             notify :message "[SIEVE] ${sender}: ${subject}"
               "xmpp:user@im.example.com";
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_4(self):
        sieve = '''
        require ["variables", "envelope", "enotify", "duplicate"];

       if envelope :matches "from" "*" { set "sender" "${1}"; }
       if header :matches "subject" "*" { set "subject" "${1}"; }
    
       # account for 'Re:' prefix
       if string :comparator "i;ascii-casemap"
         :matches "${subject}" "Re:*"
       {
         set "subject" "${1}";
       }
       if not duplicate :seconds 1800
         :uniqueid "${sender} ${subject}"
       {
         notify :message "[SIEVE] ${sender}: ${subject}"
           "xmpp:user@im.example.com";
       }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_5(self):
        sieve = '''
        require ["duplicate", "imap4flags"];

       if duplicate :header "X-Event-ID" :handle "notifier" {
         discard;
       }
       if allof (
         duplicate :header "X-Ticket-ID" :handle "support",
         address "to" "support@example.com",
         header :contains "subject" "fileserver")
       {
         setflag "\\seen";
       }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
