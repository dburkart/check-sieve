import unittest
import checksieve

class TestExamples(unittest.TestCase):

	def test_example_1(self):
		sieve = '''
			require ["copy", "environment", "imapsieve"];
		
			 if anyof (environment :is "imap.cause" "APPEND",
					   environment :is "imap.cause" "COPY")  {
				 if environment :is "imap.mailbox" "ActionItems" {
					 redirect :copy "actionitems@example.com";
				 }
			 }
		'''
		self.assertFalse(checksieve.parse_string(sieve, False))

	def test_example_2(self):
		sieve = '''
			require ["enotify", "imap4flags", "variables",
			  "environment", "imapsieve"];
	
			 if environment :matches "imap.mailbox" "*" {
				 set "mailbox" "${1}";
			 }
			
			 if allof (hasflag "\\Flagged",
					   environment :contains "imap.changedflags" "\\Flagged") {
			   notify :message "Important message in ${mailbox}"
				   "xmpp:tim@example.com?message;subject=SIEVE";
			 }
		 '''
		self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
	unittest.main()
