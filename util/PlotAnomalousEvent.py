# ------------------------------------------------------------------------------
# Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

from sys import argv

from matplotlib.pyplot import axvspan, plot, show
from numpy import loadtxt, log10

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
