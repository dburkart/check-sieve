import unittest
import checksieve

class TestServermetadata(unittest.TestCase):

    def test_servermetadata(self):
        sieve = '''
        require ["servermetadata", "variables", "envelope"];

        if servermetadata :matches
            "/private/vendor/vendor.isode/notification-uri" "*" {
            set "notif_uri" "${0}";
        }

        if not string :is "${notif_uri}" "none" {
            # :matches is used to get the MAIL FROM address
            if envelope :all :matches "from" "*" {
                set "env_from" " [really: ${1}]";
            }

            # :matches is used to get the value of the Subject header
            if header :matches "Subject" "*" {
                set "subject" "${1}";
            }
            # :matches is used to get the address from the From header
            if address :matches :all "from" "*" {
                set "from_addr" "${1}";
            }

            # notify :message "${from_addr}${env_from}: ${subject}"
            #         "${notif_uri}";
            stop;
        }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == '__main__':
    unittest.main()