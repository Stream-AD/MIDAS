from pathlib import Path

import matplotlib.pyplot as mpl
import numpy as np
import pandas as pd
from matplotlib.ticker import *

root = (Path(__file__) / '../../../..').resolve()
prefix = root
data = [
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Normal/Time/DDoS.csv'),
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Relational/Time/DDoS.csv'),
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
	[0.5, 0.175],  # Normal
	[0.5, 0.775],  # Relational
]
for i in range(len(data)):
	group = data[i].groupby('threshold').median()
	group.time /= 1e3
	mpl.scatter(group.index, group.time, label=alg_version[i], marker=marker[i])
	z = np.poly1d(np.polyfit(np.log10(group.index), group.time.array, deg=1))
	mpl.plot(group.index, z(np.log10(group.index)), ls='--')
	mpl.annotate(f"Slope* = {z.coeffs[0]:.2f}", text_pos[i], xycoords='axes fraction', ha='center', va='center')
mpl.xscale('log')
mpl.ylim(0,1.8)
mpl.xlabel('Threshold')
mpl.ylabel('Running Time (s)')
mpl.grid(True, which='major', axis='both')
mpl.legend()
mpl.tight_layout()
mpl.savefig(prefix / 'doc/Manuscript/img/Scalability.Threshold.pdf')
