import unittest
import checksieve

class TestBogus(unittest.TestCase):

	def test_bogus_argument(self):
		sieve = '''
			require ["convert"];
			convert "image/tiff" 2 ["pix-x=320","pix-y=240"];
		'''
		self.assertTrue(checksieve.parse_string(sieve, True))
	
	def test_unquoted_strings(self):
		sieve = '''
			require ["convert"];
			convert image/tiff image/jpeg ["pix-x=320","pix-y=240"];
		'''
		self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
	unittest.main()
