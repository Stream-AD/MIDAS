from pathlib import Path
import pandas as pd

def yellow_small(prefix: Path) -> None:
	"""
	This is a test function, the generated dataset is not for the RejectMIDAS algorithm.

	Two files `yellow-small_processed.csv` and `yellow-small_shape.txt` should be generated in `RejectMIDAS/data/`

	"""
	data = pd.read_csv(prefix / 'data/yellow-small.data', header=None, names=['color', 'size', 'act', 'age', 'inflated'], dtype='category')
	data.to_csv(prefix / 'data/yellow-small_processed.csv', header=False, index=False)
	with open(str(prefix / 'data/yellow-small_shape.txt'), 'w') as file:
		file.write(str(data.shape[0]))

def darpa_original(prefix: Path) -> None:
	data = pd.read_csv(prefix / 'data/darpa_original.csv', header=None, names=['src', 'dst', 'ts', 'atk'], dtype='category')
	data.atk = data.atk != '-'
	data.atk = data.atk.astype('int')
	all_site = pd.concat([data.src, data.dst])
	all_site = all_site.astype('category')
	all_site = all_site.cat.codes
	data.src = all_site[:data.shape[0]]
	data.dst = all_site[data.shape[0]:]
	data.ts = data.ts.cat.codes + 1  # Time starts from 1
	data.to_csv(prefix / 'data/darpa_processed.csv', header=False, index=False)
	with open(str(prefix / 'data/darpa_shape.txt'), 'w') as file:
		file.write(str(data.shape[0]))

def final_dataset(prefix: Path) -> None:
	data = pd.read_csv(prefix / 'data/final_dataset.csv', usecols=['Src IP', 'Dst IP', 'Timestamp', 'Label'], dtype='category')
	data.rename({
		'Src IP': 'src',
		'Dst IP': 'dst',
		'Timestamp': 'ts',
		'Label': 'atk'
	}, axis=1, inplace=True)
	data.sort_values(['ts', 'src', 'dst'], inplace=True)
	data.atk = (data.atk == 'ddos').astype('int')
	all_site = pd.concat([data.src, data.dst], copy=False).astype('category').cat.codes
	data.src = all_site[:data.shape[0]]
	data.dst = all_site[data.shape[0]:]
	data.ts = data.ts.cat.codes + 1  # Time starts from 1
	data.to_csv(prefix / 'data/final_dataset_processed.csv', header=False, index=False)
	with open(str(prefix / 'data/final_dataset_shape.txt'), 'w') as file:
		file.write(str(data.shape[0]))

if __name__ == '__main__':
	root = Path(__file__) / '../..'
	root = root.resolve()
	yellow_small(root)
	# darpa_original(root)
	# final_dataset(root)
