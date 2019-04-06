import unittest
import checksieve

class TestExamples(unittest.TestCase):
    
    def test_extended_example(self):
        sieve = '''
    #
    # Example Sieve Filter
    # Declare any optional features or extension used by the script
    #
    require ["fileinto"];

    #
    # Handle messages from known mailing lists
    # Move messages from IETF filter discussion list to filter mailbox
    #
    if header :is "Sender" "owner-ietf-mta-filters@imc.org"
            {
            fileinto "filter";  # move to "filter" mailbox
            }
    #
    # Keep all messages to or from people in my company
    #
    elsif address :DOMAIN :is ["From", "To"] "example.com"
            {
            keep;               # keep in "In" mailbox
            }

    #
    # Try and catch unsolicited email.  If a message is not to me,
    # or it contains a subject known to be spam, file it away.
    #
    elsif anyof (not address :all :contains
                   ["To", "Cc", "Bcc"] "me@example.com",
                 header :matches "subject"
                   ["*make*money*fast*", "*university*dipl*mas*"])
            {
            fileinto "spam";   # move to "spam" mailbox
            }
    else
            {
            # Move all other (non-company) mail to "personal"
            # mailbox.
            fileinto "personal";
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))
    
    def test_match_variables(self):
        sieve = '''
      require ["fileinto", "variables"];

      if header :matches "List-ID" "*<*@*" {
          fileinto "INBOX.lists.${2}"; stop;
      }

      # Imagine the header
      # Subject: [acme-users] [fwd] version 1.0 is out
      if header :matches "Subject" "[*] *" {
          # ${1} will hold "acme-users",
          # ${2} will hold "[fwd] version 1.0 is out"
          fileinto "INBOX.lists.${1}"; stop;
      }

      # Imagine the header
      # To: coyote@ACME.Example.COM
      if address :matches ["To", "Cc"] ["coyote@**.com",
              "wile@**.com"] {
          # ${0} is the matching address
          # ${1} is always the empty string
          # ${2} is part of the domain name ("ACME.Example")
          fileinto "INBOX.business.${2}"; stop;
      } else {
          # Control wouldn't reach this block if any match was
          # successful, so no match variables are set at this
          # point.
      }

      if anyof (true, address :domain :matches "To" "*.com") {
          # The second test is never evaluated, so there are
          # still no match variables set.
          stop;
      }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
    unittest.main()