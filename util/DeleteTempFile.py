from glob import glob
from os import remove
from pathlib import Path
from sys import argv

root = (Path(__file__) / '../..').resolve()

if len(argv) <= 1:
	print('Usage: python DeleteTempFile.py <pattern> [<pattern>...]')
else:
	for i in range(1, len(argv)):
		map(lambda file: remove(file), glob(str(root / f"temp/{argv[i]}")))
