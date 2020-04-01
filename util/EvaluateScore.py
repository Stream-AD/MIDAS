from pathlib import Path
from sys import argv

from joblib import dump, load
from pandas import read_csv
from sklearn.metrics import roc_auc_score

root = (Path(__file__) / '../..').resolve()

if len(argv) < 3:
	print('Print ROC-AUC to stdout and RejectMIDAS/temp/AUC[<indexRun>].txt')
	print('Usage: python EvaluateScore.py <pathGroundTruth> <pathScore> [<indexRun>]')
else:
	pathGroundTruth = Path(argv[1]).with_suffix('.bin')
	if pathGroundTruth.exists():
		y = load(pathGroundTruth)
	else:
		y = read_csv(argv[1], header=None)
		dump(y, pathGroundTruth)
	z = read_csv(argv[2], header=None)
	indexRun = argv[3] if len(argv) >= 4 else ''
	auc = roc_auc_score(y, z)
	print(f"ROC-AUC{indexRun} = {auc:.4f}")
	with open(root / f"temp/AUC{indexRun}.txt", 'w') as file:
		file.write(str(auc))
