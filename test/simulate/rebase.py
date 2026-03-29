#!/usr/bin/env python3
"""
Regenerate all .out files for the simulate test suite.

Usage:
    python3 test/simulate/rebase.py

For every RFC directory under test/simulate/, runs check-sieve --simulate
against each (.sieve, .eml) pair and writes the output (first line stripped)
to the corresponding .out file.  Existing .out files are overwritten.
New .out files are created for any pair that doesn't have one yet.
"""

import sys
from pathlib import Path

# Allow running as a top-level script from the repo root
sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from test.simulate import util


def main():
    total = 0
    for rfc_dir in util.discover_rfc_dirs():
        count = util.rebase(rfc_dir)
        print(f'Rebased {count} test(s) in {rfc_dir.name}/')
        total += count
    print(f'\nTotal: {total} .out file(s) written.')


if __name__ == '__main__':
    main()
