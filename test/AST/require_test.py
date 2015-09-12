import sys
sys.path.append('./')

import unittest
import checksieve
import util

class TestRequireAST(unittest.TestCase):

    def test_require(self):
        diff = util.diff(util.run_mock('require/01.sieve'), 'require/01.out')
        self.assertFalse(diff, msg='\n{}'.format(diff))

    def test_require_list(self):
        diff = util.diff(util.run_mock('require/02.sieve'), 'require/02.out')
        self.assertFalse(diff, msg='\n{}'.format(diff))


if __name__ == '__main__':
    unittest.main()
