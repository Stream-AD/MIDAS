from pathlib import Path
from sys import argv

import numpy as np
import pandas as pd
from sklearn.metrics import auc, roc_curve

root = (Path(__file__) / '../..').resolve()

if len(argv) < 3:
	print('Usage: python ReproduceROC.py <pathGroundTruth> <pathScore>')
else:
	y = pd.read_csv(argv[1], header=None)
	z = pd.read_csv(argv[2], header=None)
	fpr, tpr, _ = roc_curve(y, z, pos_label=1)
	print(f"ROC-AUC = {auc(fpr, tpr):.4f}")
	np.savetxt(root / 'temp/ROC.csv', np.array([fpr, tpr]).T, fmt='%f', delimiter=',')
