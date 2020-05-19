from pathlib import Path

import matplotlib.pyplot as mpl
import numpy as np
import pandas as pd

root = (Path(__file__) / '../..').resolve()
prefix = root
data = [
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Normal/Scalability/Bucket.csv'),
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Relational/Scalability/Bucket.csv'),
]
mpl.rcParams.update({
	'font.family': 'Ubuntu Condensed',
	'font.size': 13,
})
mpl.clf()
ax = mpl.axes()
marker = ['s', 'D']
alg_version = ['Normal', 'Relational']
text_pos = [
	[0.85, 0.25],  # Normal
	[0.65, 0.85],  # Relational
]
for i in range(len(data)):
	group = data[i].groupby('numColumn').median()
	group.time /= 1e6
	# group.index /= 1000
	mpl.scatter(group.index, group.time, label=alg_version[i], marker=marker[i])
	z = np.poly1d(np.polyfit(group.index, group.time.array, deg=1))
	mpl.plot(group.index, z(group.index), ls='--')
	mpl.annotate(f"Slope = {z.coeffs[0]:.2e}", text_pos[i], xycoords='axes fraction', ha='center', va='center')
mpl.ylim(bottom=0)
mpl.xlabel('# Buckets (Columns)')
mpl.ylabel('Running Time (s)')
mpl.grid(True, which='major', axis='both')
mpl.legend()
mpl.tight_layout()
mpl.savefig(prefix / 'doc/Manuscript/img/Scalability.Bucket.pdf')
