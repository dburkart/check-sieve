import sys
sys.path.append('./')

import unittest
import checksieve
import util

class TestControlAST(util.DiffTestCase):
    
    def test_simple_if(self):
        self.assertNoDiff(util.diff(util.run_mock('control/if_1.sieve'), 'control/if_1.out'))
    
    def test_if_else(self):
        self.assertNoDiff(util.diff(util.run_mock('control/if_2.sieve'), 'control/if_2.out'))
    
    def test_if_elsif_else(self):
        self.assertNoDiff(util.diff(util.run_mock('control/if_3.sieve'), 'control/if_3.out'))


if __name__ == '__main__':
    unittest.main()
