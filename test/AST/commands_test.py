import unittest
import checksieve
from . import util

class TestCommandsAST(util.DiffTestCase):
    
    def test_require(self):
        self.assertNoDiff(util.diff(util.run_mock('commands/require_single.sieve'), 'commands/require_single.out'))

    def test_require_list(self):
        self.assertNoDiff(util.diff(util.run_mock('commands/require_list.sieve'), 'commands/require_list.out'))

    def test_stop(self):
        self.assertNoDiff(util.diff(util.run_mock('commands/stop.sieve'), 'commands/stop.out'))


if __name__ == '__main__':
    unittest.main()
