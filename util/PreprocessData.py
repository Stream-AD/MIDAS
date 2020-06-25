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

from pathlib import Path

from pandas import read_csv, concat

def darpa_original(prefix: Path) -> None:
	data = read_csv(prefix / 'data/DARPA/darpa_original.csv', header=None, names=['src', 'dst', 'ts', 'atk'], dtype='category')
	data.atk = data.atk != '-'
	data.atk = data.atk.astype('int')
	# data.ts = data.ts.apply(lambda a: a[:-3]).astype('category') # Hour-based, comment this for minute-based
	all_site = concat([data.src, data.dst])
	all_site = all_site.astype('category')
	all_site = all_site.cat.codes
	data.src = all_site[:data.shape[0]]
	data.dst = all_site[data.shape[0]:]
	data.ts = data.ts.cat.codes + 1  # Time starts from 1
	data.iloc[:, :-1].to_csv(prefix / 'data/DARPA/darpa_processed.csv', header=False, index=False, line_terminator='\n')
	data.iloc[:, -1].to_csv(prefix / 'data/DARPA/darpa_ground_truth.csv', header=False, index=False, line_terminator='\n')
	with open(str(prefix / 'data/DARPA/darpa_shape.txt'), 'w') as file:
		file.write(str(data.shape[0]))

if __name__ == '__main__':
	root = (Path(__file__) / '../..').resolve()
	darpa_original(root)
