#!/usr/bin/env python3
"""Target-independent checks for a public TH11 checkout."""
from pathlib import Path
import subprocess
import sys
ROOT = Path(__file__).resolve().parents[1]
for command in [
    ['ci.py'],
    ['progress.py', '--check'], ['test-exact-oracle.py'], ['test-linked-oracle.py'],
]:
    subprocess.run([sys.executable, str(ROOT / 'scripts' / command[0]), *command[1:]], cwd=ROOT, check=True)
subprocess.run(['git', 'diff', '--check'], cwd=ROOT, check=True)
print('TH11 public checkout checks passed')
