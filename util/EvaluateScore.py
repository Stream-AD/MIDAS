import sys
import pandas as pd
import sklearn.metrics as skm

if len(sys.argv) <= 1:
	print(f"Usage: python EvaluateScore.py <pathData> <pathScore>")
else:
	y = pd.read_csv(sys.argv[1], header=None, usecols=[3])
	z = pd.read_csv(sys.argv[2], header=None)
	print(f"ROC-AUC = {skm.roc_auc_score(y, z):.4f}\t// Evaluation is over, program should return 0")
