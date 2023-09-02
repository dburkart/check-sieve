import unittest
import checksieve


class TestExamples(unittest.TestCase):
    def test_example_1a(self):
        sieve = """
            require ["envelope", "extlists", "fileinto", "spamtest",
                     "relational", "comparator-i;ascii-numeric"];
            if envelope :list "from" ":addrbook:default"
            { /* Known: allow high spam score */
                if spamtest :value "ge" :comparator "i;ascii-numeric" "8"
                {
                    fileinto "spam";
                }
            }
            elsif spamtest :value "ge" :comparator "i;ascii-numeric" "3"
            { /* Unknown: less tolerance in spam score */
                fileinto "spam";
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_1b(self):
        sieve = """
            require ["envelope", "extlists", "fileinto", "spamtest",
                "variables", "relational", "comparator-i;ascii-numeric"];
            if envelope :list "from" ":addrbook:default" {
                set "lim" "8";  /* Known: allow high spam score */
            } else {
                set "lim" "3";  /* Unknown: less tolerance in spam score */
            }
            if spamtest :value "ge" :comparator "i;ascii-numeric" "${lim}" {
                fileinto "spam";
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_2(self):
        sieve = """
            require ["extlists", "date", "enotify"];
            if currentdate :list "date"
                "tag:example.com,2011-01-01:localHolidays" {
                notify "xmpp:romeo@im.example.com";
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_3(self):
        sieve = """
            require ["extlists", "envelope", "subaddress"];

            # Submission from list members is sent to all members
            if allof (envelope :detail "to" "mylist",
                        header :list "from"
                                "tag:example.com,2010-05-28:mylist") {
                redirect :list "tag:example.com,2010-05-28:mylist";
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_4(self):
        sieve = """
            require ["variables", "extlists", "index", "reject"];
            if header :index 1 :matches "received" "*(* [*.*.*.*])*" {
                set "ip" "${3}.${4}.${5}.${6}";
                if string :list "${ip}"
                    "tag:example.com,2011-04-10:DisallowedIPs" {
                reject "Message not allowed from this IP address";
                }
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_example_5(self):
        sieve = """
            require [ "extlists", "foreverypart", "mime", "enclose" ];

            foreverypart
            {
                if header :mime :param "filename"
                    :list ["Content-Type", "Content-Disposition"]
                    "tag:example.com,2011-04-10:BadFileNameExts"
                {
                # these attachment types are executable
                enclose :subject "Warning" :text
            WARNING! The enclosed message has attachments that might be unsafe.
            These attachment types may contain a computer virus program
            that can infect your computer and potentially damage your data.

            Before clicking on these message attachments, you should verify
            with the sender that this message was sent intentionally, and
            that the attachments are safe to open.
            .
            ;
                break;
                }
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == "__main__":
    unittest.main()
