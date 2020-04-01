from pathlib import Path

import numpy as np
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
	data['ts'] = data['ts'].apply(lambda s: s[0:2] + s[3:5] + str(int(s[11:13] >= '12')))  # Month + day + hour + minute
	date = data['ts'].unique()  # This is date, not data
	data['ts'] = data['ts'].astype('category').cat.codes + 1  # Time starts from 1
	data['src'] = data['src'].astype('category').cat.codes
	data['dst'] = data['dst'].astype('category').cat.codes
	data.sort_values(['ts', 'src', 'dst'], inplace=True)
	data.to_csv(prefix / 'data/twitter_security_processed.csv', header=False, index=False)
	# I don't need the shape

	ground_truth = pd.read_excel(prefix / 'data/Ground Truth- 2009 & 2014.xlsx', sheet_name=1, names=['ts'], usecols=[0], squeeze=True)
	ground_truth[15] = pd.Timestamp('2014-06-21')  # Hard code
	ground_truth = ground_truth.apply(lambda a: a.strftime('%m%d'))
	span = np.full([ground_truth.shape[0], 2], -1)
	for i, t in enumerate(ground_truth):
		for j, d in enumerate(date):
			if d.startswith(t):
				span[i, 0] = j
				break
		for j, d in enumerate(reversed(date)):
			if d.startswith(t):
				span[i, 1] = date.shape[0] - 1 - j
				break
	span: np.ndarray = span[np.any(span != -1, axis=1), :]
	np.savetxt(prefix / 'data/twitter_security_ground_truth.csv', span, fmt='%i', delimiter=',')

if __name__ == '__main__':
	root = (Path(__file__) / '../..').resolve()
	# darpa_original(root)
	# final_dataset(root)
	Twitter_May_Aug_2014_TerrorSecurity_resolved(root)
	pass
