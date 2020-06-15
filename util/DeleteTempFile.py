# ------------------------------------------------------------------------------
# Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

from glob import glob
from os import remove
from pathlib import Path
from sys import argv

root = (Path(__file__) / '../..').resolve()

if len(argv) <= 1:
	print('Delete MIDAS/temp/<pattern>')
	print('Usage: python DeleteTempFile.py <pattern> [<pattern>...]')
else:
	for i in range(1, len(argv)):
		for file in glob(str(root / f"temp/{argv[i]}")):
			remove(file)
