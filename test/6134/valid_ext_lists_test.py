import unittest
import checksieve


class TestValidExtList(unittest.TestCase):
    def test_valid_ext_list(self):
        sieve = """
            require ["extlists", "reject"];
            if valid_ext_list ":addrbook:default"
            {
                reject "Sorry, only friends and work!";
            }
            elsif valid_ext_list [":addrbook:frens", ":addrbook:work"]
            {
                stop;
            }
        """
        self.assertFalse(checksieve.parse_string(sieve, False))


if __name__ == "__main__":
    unittest.main()
