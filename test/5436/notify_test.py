import unittest
import checksieve

class TestMailto(unittest.TestCase):

	def test_example1(self):
		sieve = '''
		require ["enotify", "variables"];
	
		  if header :contains "list-id" "knitting.example.com" {
			if header :matches "Subject" "[*] *" {
			  notify :message "From ${1} list: ${2}"
				  :importance "3"
				  "mailto:0123456789@sms.example.net?to=backup@example.com";
			}
		  }
		'''
		self.assertFalse(checksieve.parse_string(sieve, False))

if __name__ == '__main__':
	unittest.main()