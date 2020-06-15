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

from pathlib import Path
from sys import argv

from numpy import array, savetxt
from pandas import read_csv
from sklearn.metrics import auc, roc_curve

root = (Path(__file__) / '../..').resolve()

if len(argv) < 3:
	print('Print ROC-AUC to stdout and save points on ROC curve to MIDAS/<pathROC> (default temp/ROC.csv)')
	print('Usage: python ReproduceROC.py <pathGroundTruth> <pathScore> [<pathROC>]')
else:
	y = read_csv(argv[1], header=None)
	z = read_csv(argv[2], header=None)
	fpr, tpr, _ = roc_curve(y, z, pos_label=1)
	print(f"ROC-AUC = {auc(fpr, tpr):.4f}")
	pathROC = argv[3] if len(argv) >= 4 else 'temp/ROC.csv'
	savetxt(root / pathROC, array([fpr, tpr]).T, fmt='%f', delimiter=',')
