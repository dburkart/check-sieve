import unittest
import checksieve

class TestExamples(unittest.TestCase):

	def test_example_1(self):
		sieve = '''
			require ["convert"];
			convert "image/tiff" "image/jpeg" ["pix-x=320","pix-y=240"];
		'''
		self.assertFalse(checksieve.parse_string(sieve, False))

	def test_example_2(self):
		sieve = '''
			require ["mime", "fileinto", "convert"];
		    if header :mime :anychild :contenttype
					 "Content-Type" "image/tiff"
		    {
				if (convert "image/tiff" "image/jpeg" ["pix-x=320","pix-y=240"]) {
				   fileinto "INBOX.pics";
				}
		    }
		 '''
		self.assertFalse(checksieve.parse_string(sieve, False))
	
	def test_example_3(self):
		sieve = '''
			require ["mime", "foreverypart", "fileinto", "convert"];
			   foreverypart
			   {
				 if header :mime :param "filename" :contains
						   "Content-Disposition" "inline"
				 {
				   if size :over "500K"
				   {
					convert "image/tiff" "image/jpeg" ["pix-x=640","pix-y=480"];
				   } else {
				   convert "image/tiff" "image/jpeg" ["pix-x=320","pix-y=240"];
					  }
					}
				  }
		 '''
		self.assertFalse(checksieve.parse_string(sieve, False))
		
	def test_example_4(self):
		sieve = '''
			require ["mime", "foreverypart",
			"fileinto", "redirect", "convert"];

		   # The first "if" block will convert all image/tiff body parts
		   # to 640x480 jpegs and will file the message
		   # into the "INBOX.pics" mailbox as converted at this point.
		   if header :mime :anychild :contenttype
					 "Content-Type" "image/tiff"
		   {
			 convert "image/tiff" "image/jpeg" ["pix-x=640","pix-y=480"];
			 fileinto "INBOX.pics";
		   }
		
		   # The second block, the "foreverypart" loop, will convert all
		   # inline jpegs to 320x240 resolution... including any tiff body
		   # parts that had been converted in the first block, above.
		   # Therefore, any tiff that had been converted to a 640x480 jpeg
		   # will be re-converted to a 320x240 jpeg here if its
		   # Content-Disposition is specified as "inline".
		   foreverypart
		   {
			 if header :mime :param "filename" :contains
					   "Content-Disposition" "inline"
			 {
			   convert "image/jpeg" "image/jpeg" ["pix-x=320","pix-y=240"];
			 }
		   }
		
		   # The third block will take any message that contains a header
		   # field called "Mobile-Link" and redirect it to the user's
		   # mobile address.  The redirected message will include both
		   # conversions above, from block one and block two.
		   if exists "Mobile-Link"
		   {
			 redirect "joe@mobile.example.com";
		   }
		   
		   # The fourth block will file the message into "Tiff" if it
		  # contains any tiff body parts.  But because of the earlier
		  # conversion (in the first block), there will never be any
		  # tiff body parts, so this "fileinto" will never happen.
		  if header :mime :anychild :contenttype
					"Content-Type" "image/tiff"
		  {
			fileinto "Tiff";
		  }
   
		  # Now, at the end of the script processing, the Sieve
		  # processor will perform an implicit keep if none of
		  # the "fileinto" and "redirect" actions were taken.
		  # The kept message will include any conversions that
		  # were done (that is, any from the second block).
		 '''
		self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
	unittest.main()
