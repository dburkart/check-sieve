import unittest
import checksieve

class ExamplesTest(unittest.TestCase):

    def test_notify_dsn_01(self):
        sieve = '''
        require ["envelope", "envelope-dsn"];

        # Check whether SUCCESS notifications were requested,
        # irrespective of any other requests that were made
        if envelope "notify" "SUCCESS"
        {
            # do whatever
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_dsn_02(self):
        sieve = '''
        require ["envelope", "envelope-dsn", "relational",
                 "comparator-i;ascii-numeric"];

        # Check whether only FAILURE notifications were requested
        if allof ( envelope "notify" "FAILURE",
                   envelope :comparator "i;ascii-numeric"
                            :count "eq" "notify" "1"
                 )
        {
            # do whatever
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_dsn_03(self):
        sieve = '''
        require ["envelope", "envelope-dsn"];

        # See if the orcpt is an RFC822 address in the example.com
        # domain
        if envelope :matches "orcpt" "rfc822;*@example.com"
        {
            # do whatever
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_deliverby_01(self):
        sieve = '''
        require ["envelope", "envelope-deliverby", "relational",
            "comparator-i;ascii-numeric"];

        # Check to see if this message didn't make it in the time allotted by
        # the originator.
        if anyof (envelope :contains "bytimerelative" "-",
                  envelope :value "eq" :comparator "i;ascii-numeric"
                           "bytimerelative" "0")
        {
            # do whatever
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_deliverby_02(self):
        sieve = '''
        require ["envelope", "envelope-deliverby", "relational", "date",
            "variables"];
        # Check to see if this message didn't make it in the time allotted by
        # the originator.
        if currentdate :matches "iso8601" "*" {
            set "cdate" "${0}";
            if envelope :value "ge" "bytimeabsolute" "${cdate}" {
                # do whatever
            }
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_deliverby_03(self):
        sieve = '''
        require ["envelope", "envelope-deliverby", "relational", "date",
            "variables", "fileinto"];

        # If the message didn't make it in time, file it according to when it
        # should have been received
        if envelope :matches :zone "+0000" "bytimeabsolute" "*T*:*:*" {
            set "bdate" "${0}";
            set "bhour" "${2}";
            if currentdate :zone "+0000" :value "lt" "iso8601" "${bdate}" {
                fileinto "missed-${bhour}";
            }
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_notify_deliverby_04(self):
        sieve = '''
        require ["envelope", "relational", "date",
            "variables", "fileinto"];

        # If the message didn't make it in time, file it according to when it
        # should have been received
        if envelope :matches :zone "+0000" "from" "*T*:*:*" {
            set "bdate" "${0}";
            set "bhour" "${2}";
            if currentdate :zone "+0000" :value "lt" "iso8601" "${bdate}" {
                fileinto "missed-${bhour}";
            }
        }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_redirect_dsn_01(self):
        sieve = '''
        require ["copy", "redirect-dsn"];

        # Make a private copy of messages from user@example.com
        if address "from" "user@example.com"
        {
            redirect :copy :notify "NEVER" "elsewhere@example.com";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_redirect_deliverby_01(self):
        sieve = '''
        require ["copy", "redirect-deliverby"];

        # Send a copy to my cell phone, time out after 10 minutes
        if address "from" "user@example.com"
        {
            redirect :copy :bytimerelative 600 "cellphone@example.com";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_redirect_deliverby_02(self):
        sieve = '''
        require ["copy", "redirect-deliverby", "date", "variables",
            "relational", "comparator-i;ascii-numeric"];

        # Send a copy to my cell phone to be delivered before 10PM
        if currentdate :value "lt"
                       :comparator "i;ascii-numeric" "hour" "22"
        {
            if currentdate :matches "date" "*" {set "date" "${0}";}
            if currentdate :matches "zone" "*" {set "zone" "${0}";}
            redirect :copy :bytimeabsolute "${date}T20:00:00${zone}"
                     :bymode "return" "cellphone@example.com";
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()
