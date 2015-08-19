import sys
sys.path.append('./')

import unittest
import checksieve

class TestActions(unittest.TestCase):

    def test_foreverypart(self):
        sieve = '''
            require ["foreverypart"];

            foreverypart { }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_foreverypart_no_require(self):
        sieve = '''
            foreverypart { }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_foreverypart_with_name(self):
        sieve = '''
            require "foreverypart";

            foreverypart :name "Cc" {

            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_foreverypart_invalid_tag(self):
        sieve = '''
            require ["foreverypart", "fileinto"];

            foreverypart :foo "bar" {
                fileinto "Nonsense";
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_break(self):
        sieve = '''
            require "foreverypart";

            foreverypart
            {
                break;
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_break_no_require(self):
        sieve = '''
            foreverypart
            {
                break;
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_break_with_name(self):
        sieve = '''
            require "foreverypart";

            foreverypart {
                break :name "Subject";
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_break_with_invalid_tag(self):
        sieve = '''
            require "foreverypart";

            foreverypart {
                break :foo "bar";
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_mime(self):
        sieve = '''
            require ["mime", "foreverypart", "fileinto"];

           foreverypart
           {
               if allof (
                 header :mime :param "filename" :contains
                    "Content-Disposition" "important",
                 header :mime :subtype "Content-Type" "pdf",
                 size :over "100K")
               {
                   fileinto "INBOX.important";
                   break;
               }
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_mime_no_require(self):
        sieve = '''
            require ["foreverypart", "fileinto"];

           foreverypart
           {
               if allof (
                 header :mime :param "filename" :contains
                    "Content-Disposition" "important",
                 header :mime :subtype "Content-Type" "pdf",
                 size :over "100K")
               {
                   fileinto "INBOX.important";
                   break;
               }
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_replace(self):
        sieve = '''
               require [ "foreverypart", "mime", "replace" ];
           foreverypart
           {
             if anyof (
                    header :mime :contenttype :is
                      "Content-Type" "application/exe",
                    header :mime :param "filename"
                      :matches ["Content-Type", "Content-Disposition"] "*.com" )
             {
               replace "Executable attachment removed by user filter";
             }
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_replace_no_require(self):
        sieve = '''
               require [ "foreverypart", "mime" ];
           foreverypart
           {
             if anyof (
                    header :mime :contenttype :is
                      "Content-Type" "application/exe",
                    header :mime :param "filename"
                      :matches ["Content-Type", "Content-Disposition"] "*.com" )
             {
               replace "Executable attachment removed by user filter";
             }
           }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_enclose(self):
        sieve = '''
require [ "foreverypart", "mime", "enclose" ];

foreverypart
{
 if header :mime :param "filename"
    :matches ["Content-Type", "Content-Disposition"]
      ["*.com", "*.exe", "*.vbs", "*.scr",
       "*.pif", "*.hta", "*.bat", "*.zip" ]
 {
   # these attachment types are executable
   enclose :subject "Warning" text:
WARNING! The enclosed message contains executable attachments.
These attachment types may contain a computer virus program
that can infect your computer and potentially damage your data.

Before clicking on these message attachments, you should verify
with the sender that this message was sent by them and not a
computer virus.
.
;
   break;
 }
}
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_enclose_no_require(self):
        sieve = '''
require [ "foreverypart", "mime" ];

foreverypart
{
 if header :mime :param "filename"
    :matches ["Content-Type", "Content-Disposition"]
      ["*.com", "*.exe", "*.vbs", "*.scr",
       "*.pif", "*.hta", "*.bat", "*.zip" ]
 {
   # these attachment types are executable
   enclose :subject "Warning" text:
WARNING! The enclosed message contains executable attachments.
These attachment types may contain a computer virus program
that can infect your computer and potentially damage your data.

Before clicking on these message attachments, you should verify
with the sender that this message was sent by them and not a
computer virus.
.
;
   break;
 }
}
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_extracttext(self):
        sieve = '''
           require ["mime", "variables", "extracttext", "foreverypart"];

           if header :contains "from" "boss@example.org"
           {
             # :matches is used to get the value of the Subject header
             if header :matches "Subject" "*"
             {
               set "subject" "${1}";
             }

             # extract the first 100 characters of the first text/* part
             foreverypart
             {
               if header :mime :type :is "Content-Type" "text"
               {
                 extracttext :first 100 "msgcontent";
                 break;
               }
             }

             # if it's not a 'for your information' message
             if not header :contains "subject" "FYI:"
             {
               # do something using ${subject} and ${msgcontent}
               # such as sending a notification using a
               # notification extension
             }
           }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_extracttext_no_require(self):
        sieve = '''
           require ["mime", "variables", "foreverypart"];

           if header :contains "from" "boss@example.org"
           {
             # :matches is used to get the value of the Subject header
             if header :matches "Subject" "*"
             {
               set "subject" "${1}";
             }

             # extract the first 100 characters of the first text/* part
             foreverypart
             {
               if header :mime :type :is "Content-Type" "text"
               {
                 extracttext :first 100 "msgcontent";
                 break;
               }
             }

             # if it's not a 'for your information' message
             if not header :contains "subject" "FYI:"
             {
               # do something using ${subject} and ${msgcontent}
               # such as sending a notification using a
               # notification extension
             }
           }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

if __name__ == '__main__':
    unittest.main()
