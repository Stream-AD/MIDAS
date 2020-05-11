from pathlib import Path

import matplotlib.pyplot as mpl
import pandas as pd
from matplotlib.ticker import *
from numpy import linspace

def Draw(prefix: Path, dataset_name: str, legend_pos: str):
	summary = pd.DataFrame(columns=['alg_name', 'time', 'auc'])
	summary.alg_name = summary.alg_name.astype(str)

	data = pd.read_csv(prefix / f"doc/Experiment/MIDAS/Normal/Time,AUC/{dataset_name}.csv")
	summary = summary.append(data.median()[['time', 'auc']], ignore_index=True)
	summary.iloc[-1].alg_name = 'MIDAS.Normal'

	data = pd.read_csv(prefix / f"doc/Experiment/MIDAS/Relational/Time,AUC/{dataset_name}.csv")
	summary = summary.append(data.median()[['time', 'auc']], ignore_index=True)
	summary.iloc[-1].alg_name = 'MIDAS.Relational'

	data = pd.read_csv(prefix / f"doc/Experiment/RejectMIDAS/Normal/AUC/{dataset_name}.csv")
	group = data.groupby('threshold')
	max_auc_index = group.median()['auc'].argmax()
	indices = list(data.groupby('threshold').groups.keys())
	group: pd.DataFrame = group.get_group(indices[max_auc_index])
	summary = summary.append(pd.Series(dtype=float), ignore_index=True)
	summary.iloc[-1].auc = group['auc'].median()
	data = pd.read_csv(prefix / f"doc/Experiment/RejectMIDAS/Normal/Time/{dataset_name}.csv")
	group = data.groupby('threshold').get_group(indices[max_auc_index])
	summary.iloc[-1].time = group['time'].median()
	summary.iloc[-1].alg_name = 'RejectMIDAS.Normal'

	data = pd.read_csv(prefix / f"doc/Experiment/RejectMIDAS/Relational/AUC/{dataset_name}.csv")
	group = data.groupby('threshold')
	max_auc_index = group.median()['auc'].argmax()
	indices = list(data.groupby('threshold').groups.keys())
	group: pd.DataFrame = group.get_group(indices[max_auc_index])
	summary = summary.append(pd.Series(dtype=float), ignore_index=True)
	summary.iloc[-1].auc = group['auc'].median()
	data = pd.read_csv(prefix / f"doc/Experiment/RejectMIDAS/Relational/Time/{dataset_name}.csv")
	group = data.groupby('threshold').get_group(indices[max_auc_index])
	summary.iloc[-1].time = group['time'].median()
	summary.iloc[-1].alg_name = 'RejectMIDAS.Relational'

	data = pd.read_csv(prefix / f"doc/Experiment/SedanSpot/Time,AUC/{dataset_name}.csv")
	data.time = data.time * 1000
	group = data.groupby(['numSample', 'numWalk'])
	max_auc_index = group.median()['auc'].apply(lambda b: max(b, 1 - b)).argmax()
	indices = list(group.groups.keys())
	summary = summary.append(group.get_group(indices[max_auc_index]).median()[['time', 'auc']], ignore_index=True)
	summary['alg_name'] = ['MIDAS.Normal', 'MIDAS.Relational', 'RejectMIDAS.Normal', 'RejectMIDAS.Relational', 'SedanSpot']

	summary.time /= 1000
	summary.auc = summary.auc.apply(lambda a: max(a, 1 - a))
	print(summary, '\n')

	mpl.rcParams.update({
		'font.family': 'Ubuntu Condensed',
		'font.size': 13,
	})
	marker = ['s', 'D', 'v', '^', 'o']
	mpl.clf()
	ax = mpl.axes()
	for i in range(summary.shape[0]):
		mpl.scatter(summary.time[i], summary.auc[i], label=summary.alg_name[i], marker=marker[i], s=100)
		mpl.annotate(f"{summary.auc[i]:.2f}", [summary.time[i], summary.auc[i] - 0.025], ha='center', va='center')
		mpl.annotate(f"{summary.time[i]:.3g}s", [summary.time[i], summary.auc[i] - 0.045], ha='center', va='center')
	mpl.xscale('log')
	mpl.yscale('linear')
	ax.xaxis.set_major_formatter(FormatStrFormatter('%g'))
	mpl.xlim(0.1, 1000)
	mpl.yticks(linspace(0.5, 1, 11))
	mpl.ylim(0.5, 1)
	mpl.grid(True, which='major', ls='-')
	mpl.grid(True, which='minor', ls='--')
	mpl.legend(loc=legend_pos)
	mpl.xlabel('Running Time (s)')
	mpl.ylabel('ROC-AUC')
	mpl.tight_layout()
	mpl.savefig(prefix / f"doc/Manuscript/img/TimeAUC.{dataset_name}.pdf")
	pass

if __name__ == '__main__':
	root = (Path(__file__) / '../../../..').resolve()
	Draw(root, 'Darpa', 'lower left')
	Draw(root, 'DDoS', 'lower right')
