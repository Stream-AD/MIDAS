from pathlib import Path

import matplotlib.pyplot as mpl
import numpy as np

root = (Path(__file__) / '../..').resolve()

mpl.clf()
truth = np.loadtxt(root / 'data/twitter_security_ground_truth.csv', int, delimiter=',')
for x in truth:
	mpl.axvspan(x[0], x[1], color='orange')
y = np.loadtxt(root / 'temp/Score.txt', float)
mpl.plot(range(1, y.shape[0]), np.log10(y[1:]))
mpl.show()
