import matplotlib.pyplot as mpl
import numpy as np

mpl.clf()
y = np.loadtxt('../temp/Score.txt', float)
mpl.plot(range(1, y.shape[0]), np.log10(y[1:]))
bar = np.loadtxt('../data/twitter_security_ground_truth.txt', int)
for x in bar:
	mpl.axvline(x)
