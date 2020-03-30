from pathlib import Path
from sys import argv

import pandas as pd
from sklearn.metrics import roc_auc_score

root = (Path(__file__) / '../..').resolve()

if len(argv) < 3:
	print(f"Usage: python EvaluateScore.py <pathGroundTruth> <pathScore> [<indexRun>]")
else:
	y = pd.read_csv(argv[1], header=None)
	z = pd.read_csv(argv[2], header=None)
	indexRun = argv[3] if len(argv) >= 4 else ''
	auc = roc_auc_score(y, z)
	print(f"ROC-AUC{indexRun} = {auc:.4f}")
	with open(root / f"temp/AUC{indexRun}.txt", 'w') as file:
		file.write(str(auc))
