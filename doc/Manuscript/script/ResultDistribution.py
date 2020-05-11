from pathlib import Path

import matplotlib.pyplot as mpl
import pandas as pd

root = (Path(__file__) / '../../../..').resolve()
prefix = root
mpl.rcParams.update({
	'font.family': 'Ubuntu Condensed',
	'font.size': 13,
})
cycle = mpl.rcParams['axes.prop_cycle'].by_key()['color']
mpl.clf()
i = 0
for alg_version in ['Normal', 'Relational']:
	for dataset_name in ['Darpa', 'DDoS']:
		data = pd.read_csv(prefix / f"doc/Experiment/RejectMIDAS/{alg_version}/AUC/{dataset_name}.csv")
		group = data.groupby('threshold').auc
		median_group = group.median()
		max_group = group.max()
		min_group = group.min()
		mpl.errorbar(median_group.index * (1 + 0.2 * i), median_group, label=f"{alg_version} on {dataset_name}", marker='.', lw=1, yerr=[median_group - min_group, max_group - median_group])
		mpl.scatter(median_group.index[median_group.argmax()] * (1+0.2*i), median_group.max(), marker='x')
		i += 1
# mpl.ylim(0.9,1)
mpl.xscale('log', subsx=[])
mpl.xlabel('Threshold')
mpl.ylabel('ROC-AUC')
mpl.grid(axis='y')
mpl.tight_layout()
mpl.legend()
mpl.savefig(prefix / 'doc/Manuscript/img/Distribution.pdf')
