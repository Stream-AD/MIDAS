from pathlib import Path

import pandas as pd

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
	data.iloc[:, :-1].to_csv(prefix / 'data/darpa_processed.csv', header=False, index=False)
	data.iloc[:, -1].to_csv(prefix / 'data/darpa_ground_truth.csv', header=False, index=False)
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
	data.iloc[:, :-1].to_csv(prefix / 'data/final_dataset_processed.csv', header=False, index=False)
	data.iloc[:, -1].to_csv(prefix / 'data/final_dataset_ground_truth.csv', header=False, index=False)
	with open(str(prefix / 'data/final_dataset_shape.txt'), 'w') as file:
		file.write(str(data.shape[0]))

def Twitter_May_Aug_2014_TerrorSecurity_resolved(prefix: Path):
	data = pd.read_csv(prefix / 'data/Twitter_May_Aug_2014_TerrorSecurity_resolved.txt', sep=' ', header=None, names=['ts', 'src', 'dst'])
	data = data.append(data.rename(columns={'src': 'dst', 'dst': 'src'}))
	data['ts'] = data['ts'].apply(lambda s: s[0:2] + s[3:5])
	date = data['ts'].unique()  # This is date, not data
	data['ts'] = data['ts'].astype('category').cat.codes + 1  # Time starts from 1
	data['src'] = data['src'].astype('category').cat.codes
	data['dst'] = data['dst'].astype('category').cat.codes
	data.sort_values(['ts', 'src', 'dst'], inplace=True)
	data.to_csv(prefix / 'data/twitter_security_processed.csv', header=False, index=False)
	# I don't need the shape

	date = dict(zip(date, range(date.shape[0])))  # Time starts from 1, but first row is row 0
	ground_truth = pd.read_excel(prefix / 'data/Ground Truth- 2009 & 2014.xlsx', sheet_name=1, names=['ts'], usecols=[0])
	ground_truth.iloc[15, 0] = pd.Timestamp('2014-06-21')  # Hard code
	ground_truth['ts'] = ground_truth['ts'].apply(lambda a: a.strftime('%m%d'))
	ground_truth['ts'] = ground_truth['ts'].apply(lambda a: date.get(a, None))
	ground_truth.dropna(inplace=True)
	ground_truth = ground_truth.astype(int)
	ground_truth.to_csv(prefix / 'data/twitter_security_ground_truth.txt', header=False, index=False)

if __name__ == '__main__':
	root = (Path(__file__) / '../..').resolve()
	# darpa_original(root)
	# final_dataset(root)
	Twitter_May_Aug_2014_TerrorSecurity_resolved(root)
	pass
