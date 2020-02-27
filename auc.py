from sklearn import metrics
from sklearn.metrics import precision_recall_curve
import pandas as pd

data = pd.read_csv("darpa_original.csv", names=['src', 'dst', 'time', 'label']) 
is_anom = data.label != '-'

scores = pd.read_csv("scores.txt", header=None, squeeze=True)
fpr, tpr, _ = metrics.roc_curve(is_anom, scores)
auc = metrics.roc_auc_score(is_anom, scores)
precision, recall, _ = metrics.precision_recall_curve(is_anom, scores)
print("AUC: ", auc)
