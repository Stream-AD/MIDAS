from pathlib import Path

import matplotlib.pyplot as mpl
import numpy as np
import pandas as pd
from matplotlib.ticker import *

root = (Path(__file__) / '../..').resolve()
prefix = root
data = [
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Normal/Scalability/Edge.csv'),
	pd.read_csv(prefix / 'doc/Experiment/RejectMIDAS/Relational/Scalability/Edge.csv'),
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
	[0.85, 0.6],
	[0.65, 0.85],
]
for i in range(len(data)):
	group = data[i].groupby('numRecord').median()
	group.time /= 1e6
	mpl.scatter(group.index, group.time, label=alg_version[i], marker=marker[i])
	z = np.poly1d(np.polyfit(np.log10(group.index), np.log10(group.time.array), deg=1))
	mpl.plot(group.index, np.power(10, z(np.log10(group.index))),ls='--')
	mpl.annotate(f"Slope = {z.coeffs[0]:.2f}",text_pos[i],xycoords='axes fraction', ha='center',va='center')
mpl.loglog()
ax.xaxis.set_major_formatter(EngFormatter())
ax.yaxis.set_major_formatter(FormatStrFormatter('%g'))
mpl.ylim(top=1)
mpl.xlabel('# Edges (Records)')
mpl.ylabel('Running Time (s)')
mpl.grid(True, which='major', axis='both')
mpl.legend()
mpl.tight_layout()
mpl.savefig(prefix / 'doc/Manuscript/img/Scalability.Edge.pdf')