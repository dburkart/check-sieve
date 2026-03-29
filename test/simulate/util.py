import os
import subprocess
import unittest
from tempfile import NamedTemporaryFile


class DiffTestCase(unittest.TestCase):
    def assertNoDiff(self, diff):
        self.assertFalse(diff, msg='\n{}'.format(diff))


def run_simulate(sieve_filename, eml_filename):
    current_dir = os.path.dirname(os.path.realpath(__file__))
    sieve_path = os.path.join(current_dir, 'mock', sieve_filename)
    eml_path = os.path.join(current_dir, 'mock', eml_filename)
    check_sieve_path = os.path.join(current_dir, '../..', 'check-sieve')

    return subprocess.check_output(
        [check_sieve_path, sieve_path, '--simulate', eml_path],
        stderr=subprocess.STDOUT
    )


def diff(output, expected_filename):
    current_dir = os.path.dirname(os.path.realpath(__file__))
    out_path = os.path.join(current_dir, 'mock', expected_filename)

    temp = NamedTemporaryFile(delete=False)
    # Strip the first line (contains file paths that vary by environment)
    lines = output.split(b'\n')
    stripped = b'\n'.join(lines[1:])
    temp.write(stripped)
    temp.close()

    # Also strip first line of expected output for comparison
    with open(out_path, 'rb') as f:
        expected_lines = f.read().split(b'\n')
    expected_stripped = b'\n'.join(expected_lines[1:])
    temp_expected = NamedTemporaryFile(delete=False)
    temp_expected.write(expected_stripped)
    temp_expected.close()

    result = subprocess.Popen(
        ["/usr/bin/diff", "-ru", temp.name, temp_expected.name],
        stdout=subprocess.PIPE
    ).communicate()[0]

    os.unlink(temp.name)
    os.unlink(temp_expected.name)

    return result
