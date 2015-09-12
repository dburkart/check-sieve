import os
import subprocess
from tempfile import NamedTemporaryFile

def run_mock(filename):
    current_dir = os.path.dirname(os.path.realpath(__file__))
    test_path = os.path.join(current_dir, 'mock', filename)
    check_sieve_path = os.path.join(current_dir, '../..', 'check-sieve')

    return subprocess.check_output([check_sieve_path, '--trace-tree', test_path])

def diff(output, filename):
    current_dir = os.path.dirname(os.path.realpath(__file__))
    out_path = os.path.join(current_dir, 'mock', filename)
    check_sieve_path = os.path.join(current_dir, '../..', 'check-sieve')

    diff = False

    temp = NamedTemporaryFile(delete=False)
    temp.write(output)
    temp.close()

    diff = subprocess.Popen(["/usr/bin/diff", "-ru", temp.name, out_path], stdout=subprocess.PIPE).communicate()[0]

    return diff