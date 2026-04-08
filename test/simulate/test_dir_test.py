import subprocess
import tempfile
import unittest
from pathlib import Path

CHECK_SIEVE = Path(__file__).resolve().parents[2] / 'check-sieve'

SIEVE = 'keep;\n'
EML   = 'From: test@example.com\nTo: you@example.com\nSubject: hi\n\nBody\n'

def _run(args, **kwargs):
    return subprocess.run([str(CHECK_SIEVE)] + args,
                          capture_output=True, text=True, **kwargs)

class TestDirFlag(unittest.TestCase):

    def setUp(self):
        self._tmpdir = tempfile.TemporaryDirectory()
        self.d = Path(self._tmpdir.name)
        (self.d / 'test.sieve').write_text(SIEVE)
        (self.d / 'msg.eml').write_text(EML)

    def tearDown(self):
        self._tmpdir.cleanup()

    def test_no_out_file_shows_diff(self):
        r = _run([str(self.d / 'test.sieve'), '--test-dir', str(self.d)])
        self.assertNotEqual(r.returncode, 0)
        self.assertIn('+', r.stdout)

    def test_rebase_creates_out(self):
        _run([str(self.d / 'test.sieve'), '--rebase', str(self.d)])
        self.assertTrue((self.d / 'msg.out').exists())

    def test_after_rebase_passes(self):
        _run([str(self.d / 'test.sieve'), '--rebase', str(self.d)])
        r = _run([str(self.d / 'test.sieve'), '--test-dir', str(self.d)])
        self.assertEqual(r.returncode, 0)

    def test_mismatch_fails(self):
        (self.d / 'msg.out').write_text('bogus content\n')
        r = _run([str(self.d / 'test.sieve'), '--test-dir', str(self.d)])
        self.assertNotEqual(r.returncode, 0)
        self.assertIn('failed', r.stderr)

    def test_missing_sieve_arg(self):
        r = _run(['--test-dir', str(self.d)])
        self.assertNotEqual(r.returncode, 0)

if __name__ == '__main__':
    unittest.main()
