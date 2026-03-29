import os
import subprocess
import unittest
from pathlib import Path
from tempfile import NamedTemporaryFile


SIMULATE_DIR = Path(__file__).resolve().parent
CHECK_SIEVE = SIMULATE_DIR / '../..' / 'check-sieve'


class DiffTestCase(unittest.TestCase):
    def assertNoDiff(self, diff):
        self.assertFalse(diff, msg='\n{}'.format(diff))


def discover_rfc_dirs():
    """Return sorted list of rfc* subdirectory Paths under test/simulate/."""
    return sorted(p for p in SIMULATE_DIR.iterdir()
                  if p.is_dir() and p.name.startswith('rfc'))


def discover_tests(rfc_dir):
    """
    Yield (sieve_path, eml_path, out_path) triples for every .sieve/.eml
    combination found in rfc_dir.  out_path may not exist yet (rebase creates it).
    """
    rfc_dir = Path(rfc_dir)
    sieve_files = sorted(rfc_dir.glob('*.sieve'))
    eml_files = sorted(rfc_dir.glob('*.eml'))
    for sieve in sieve_files:
        for eml in eml_files:
            out_name = f'{sieve.stem}_{eml.stem}.out'
            yield sieve, eml, rfc_dir / out_name


def run_simulate(sieve_path, eml_path):
    """Run check-sieve --simulate and return output bytes."""
    return subprocess.check_output(
        [str(CHECK_SIEVE), str(sieve_path), '--simulate', str(eml_path)],
        stderr=subprocess.STDOUT
    )


def _strip_first_line(data):
    """Strip the first line (contains environment-specific file paths)."""
    lines = data.split(b'\n')
    return b'\n'.join(lines[1:])


def diff(output, out_path):
    """
    Compare output bytes against the expected file at out_path.
    The .out file stores content without the first line (environment-specific
    paths), so we strip the first line only from the live binary output.
    Returns empty bytes if identical, diff output otherwise.
    """
    out_path = Path(out_path)

    actual = _strip_first_line(output)
    with open(out_path, 'rb') as f:
        expected = f.read()

    temp_actual = NamedTemporaryFile(delete=False)
    temp_actual.write(actual)
    temp_actual.close()

    temp_expected = NamedTemporaryFile(delete=False)
    temp_expected.write(expected)
    temp_expected.close()

    result = subprocess.Popen(
        ['/usr/bin/diff', '-ru', temp_actual.name, temp_expected.name],
        stdout=subprocess.PIPE
    ).communicate()[0]

    os.unlink(temp_actual.name)
    os.unlink(temp_expected.name)

    return result


def rebase(rfc_dir):
    """
    Regenerate all .out files for every (sieve, eml) pair in rfc_dir.
    Returns the number of files written.
    """
    count = 0
    for sieve, eml, out_path in discover_tests(rfc_dir):
        output = run_simulate(sieve, eml)
        stripped = _strip_first_line(output)
        out_path.write_bytes(stripped)
        count += 1
    return count
