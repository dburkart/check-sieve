import unittest
import checksieve

class TestNoRequire(unittest.TestCase):

	def test_no_require(self):
		sieve = '''
			require ["copy", "environment"];
		
			 if anyof (environment :is "imap.cause" "APPEND",
					   environment :is "imap.cause" "COPY")  {
				 if environment :is "imap.mailbox" "ActionItems" {
					 redirect :copy "actionitems@example.com";
				 }
			 }
		'''
		self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
	unittest.main()
