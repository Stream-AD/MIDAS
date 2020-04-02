from pathlib import Path
from sys import argv

from numpy import array, savetxt
from pandas import read_csv
from sklearn.metrics import auc, roc_curve

root = (Path(__file__) / '../..').resolve()

if len(argv) < 3:
	print('Print ROC-AUC to stdout and save points on ROC curve to RejectMIDAS/<pathROC> (default temp/ROC.csv)')
	print('Usage: python ReproduceROC.py <pathGroundTruth> <pathScore> [<pathROC>]')
else:
	y = read_csv(argv[1], header=None)
	z = read_csv(argv[2], header=None)
	fpr, tpr, _ = roc_curve(y, z, pos_label=1)
	print(f"ROC-AUC = {auc(fpr, tpr):.4f}")
	pathROC = argv[3] if len(argv) >= 4 else 'temp/ROC.csv'
	savetxt(root / pathROC, array([fpr, tpr]).T, fmt='%f', delimiter=',')
