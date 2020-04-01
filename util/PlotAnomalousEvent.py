from pathlib import Path
from sys import argv

from matplotlib.pyplot import plot, show, axvspan
from numpy import log10, loadtxt

root = (Path(__file__) / '../..').resolve()

if len(argv) < 2:
	print('Plot line chart for timestamps vs anomaly scores, with optional vertical lines for important events')
	print('Usage: python PlotAnomalousEvent.py <pathScore> [<pathEvent>]')
else:
	y = loadtxt(argv[1], float)
	plot(range(1, y.shape[0]), log10(y[1:]))  # Usually first timestamp only has score == 0
	if len(argv) >= 3:
		for x in loadtxt(argv[2], int, delimiter=','):
			axvspan(x[0], x[1], color='orange')
	show()
