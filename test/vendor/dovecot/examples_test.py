import unittest
import checksieve


class TestExamples(unittest.TestCase):
    def test_pipe_01(self):
        sieve = '''
            require [ "vnd.dovecot.pipe", "subaddress", "envelope" ];

   if envelope :detail "to" "request"
   {
     pipe "request-handler";
   }

        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_pipe_02(self):
        sieve = '''require [ "vnd.dovecot.pipe", "copy" ];

   if address "to" "snailmail@example.com"
   {
     pipe :copy "printer" ["A4", "draft"];
   }'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_filter_01(self):
        sieve = '''require [ "vnd.dovecot.filter", "fileinto" ];

   if header "content-language" "nl"
   {
     filter "translator" ["nl", "en"];
     fileinto "Translated";
     stop;
   }'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_filter_02(self):
        sieve = '''require [ "vnd.dovecot.filter", "fileinto" ];

   if header "content-language" "nl"
   {
     if filter "translator" ["nl", "en"]
     {
       fileinto "Translated";
       stop;
     }
   }'''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_execute_01(self):
        sieve = '''require [ "vnd.dovecot.execute", "vacation", "variables",
             "envelope" ];

   if envelope :localpart :matches "to" "*"
   {
     set "recipient" "${1}";
   }

   if execute :output "vacation_message" "onvacation" "${recipient}"
   {
     vacation "${vacation_message}";
   }'''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()
