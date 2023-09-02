import unittest
import checksieve


class TestProtonEval(unittest.TestCase):
    def test_eval(self):
        sieve = '''
            require "variables";
            require "fileinto";
            require "vnd.proton.eval";
            # do a match test on the sender address
            if header :matches "from" "*" {
                # create a variable called length, containing the length of the first     
                # matching variable
                set :length "length" "${1}"; 
                # Create a variable called fileintovar containing the result of the expression written below
                set :eval "fileintovar" "${length} * 25 - 1 / 8+3";
                fileinto "${fileintovar}";
            }
        '''
        self.assertFalse(checksieve.parse_string(sieve, False))

    def test_eval_no_require(self):
        sieve = '''
            require "variables";
            require "fileinto";
            # do a match test on the sender address
            if header :matches "from" "*" {
                # create a variable called length, containing the length of the first     
                # matching variable
                set :length "length" "${1}"; 
                # Create a variable called fileintovar containing the result of the expression written below
                set :eval "fileintovar" "${length} * 25 - 1 / 8+3";
                fileinto "${fileintovar}";
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))

    def test_eval_no_variables_require(self):
        sieve = '''
            require "fileinto";
            require "vnd.proton.eval";
            # do a match test on the sender address
            if header :matches "from" "*" {
                # create a variable called length, containing the length of the first     
                # matching variable
                set :length "length" "${1}"; 
                # Create a variable called fileintovar containing the result of the expression written below
                set :eval "fileintovar" "${length} * 25 - 1 / 8+3";
                fileinto "${fileintovar}";
            }
        '''
        self.assertTrue(checksieve.parse_string(sieve, True))


if __name__ == '__main__':
    unittest.main()
