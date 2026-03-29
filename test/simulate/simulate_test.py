"""
Auto-discovered simulation tests.

Test cases are generated dynamically from the RFC subdirectories under
test/simulate/.  For each RFC directory, every .sieve file is paired with
every .eml file in the same directory; the expected output is read from the
corresponding .out file (named <sieve_stem>_<eml_stem>.out).

To add a new test:
  1. Drop a .sieve file into the appropriate RFC directory.
  2. Run `make rebase` to generate the .out files.
  3. Review the generated .out files and commit.

No edits to this file are required.
"""

import unittest
from . import util


def _make_test(sieve_path, eml_path, out_path):
    def test(self):
        output = util.run_simulate(sieve_path, eml_path)
        self.assertNoDiff(util.diff(output, out_path))
    test.__doc__ = f'{sieve_path.name} × {eml_path.name}'
    return test


for _rfc_dir in util.discover_rfc_dirs():
    _class_name = f'TestSimulate_{_rfc_dir.name}'
    _methods = {}
    for _sieve, _eml, _out in util.discover_tests(_rfc_dir):
        if not _out.exists():
            continue
        _test_name = f'test_{_sieve.stem}_{_eml.stem}'
        _methods[_test_name] = _make_test(_sieve, _eml, _out)
    _cls = type(_class_name, (util.DiffTestCase,), _methods)
    globals()[_class_name] = _cls


if __name__ == '__main__':
    unittest.main()
