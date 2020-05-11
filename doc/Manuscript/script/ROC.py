from glob import glob
from pathlib import Path

import matplotlib.pyplot as mpl
import pandas as pd

def NormalDarpa(prefix: Path):
	data = [
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/RejectMIDAS/Normal/ROC/Darpa.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/MIDAS/Normal/ROC/Darpa.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/SedanSpot/ROC/Darpa.*.csv')), header=None),
	]
	auc = [0.948974, 0.904275, 0.6395]
	text_pos = [
		[0, 0.95],
		[0.2, 0.8],
		[0.35, 0.5],
	]

	mpl.clf()
	for i in range(len(data)):
		mpl.plot(data[i][0], data[i][1], label=alg_name[i], c=color[i], ls=line_style[i])
		mpl.text(text_pos[i][0], text_pos[i][1], f"AUC = {auc[i]:.4f}", c=color[i])
	mpl.legend(loc='lower right')
	mpl.xlabel('False Positive Rate')
	mpl.ylabel('True Positive Rate')
	mpl.tight_layout()
	mpl.savefig(prefix / 'doc/Manuscript/img/ROC.Normal.Darpa.pdf', transparent=False)

def RelationalDarpa(prefix: Path):
	data = [
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/RejectMIDAS/Relational/ROC/Darpa.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/MIDAS/Relational/ROC/Darpa.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/SedanSpot/ROC/Darpa.*.csv')), header=None),
	]
	auc = [0.98535, 0.951446, 0.6395]
	text_pos = [
		[-0.025, 0.975],
		[0.2, 0.875],
		[0.35, 0.5],
	]

	mpl.clf()
	for i in range(len(data)):
		mpl.plot(data[i][0], data[i][1], label=alg_name[i], c=color[i], ls=line_style[i])
		mpl.text(text_pos[i][0], text_pos[i][1], f"AUC = {auc[i]:.4f}", c=color[i])
	mpl.legend(loc='lower right')
	mpl.xlabel('False Positive Rate')
	mpl.ylabel('True Positive Rate')
	mpl.tight_layout()
	mpl.savefig(prefix / 'doc/Manuscript/img/ROC.Relational.Darpa.pdf', transparent=False)

def NormalDDoS(prefix: Path):
	data = [
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/RejectMIDAS/Normal/ROC/DDoS.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/MIDAS/Normal/ROC/DDoS.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/SedanSpot/ROC/DDoS.*.csv')), header=None),
	]
	auc = [0.985461, 0.755574, 0.9565]
	text_pos = [
		[-0.025, 0.975],
		[0.375, 0.7],
		[0.25, 0.9],
	]

	mpl.clf()
	for i in range(len(data)):
		mpl.plot(data[i][0], data[i][1], label=alg_name[i], c=color[i], ls=line_style[i])
		mpl.text(text_pos[i][0], text_pos[i][1], f"AUC = {auc[i]:.4f}", c=color[i])
	mpl.legend(loc='lower right')
	mpl.xlabel('False Positive Rate')
	mpl.ylabel('True Positive Rate')
	mpl.tight_layout()
	mpl.savefig(prefix / 'doc/Manuscript/img/ROC.Normal.DDoS.pdf', transparent=False)

def RelationalDDoS(prefix: Path):
	data = [
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/RejectMIDAS/Relational/ROC/DDoS.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/MIDAS/Relational/ROC/DDoS.*.csv')), header=None),
		pd.read_csv(*glob(str(prefix / 'doc/Experiment/SedanSpot/ROC/DDoS.*.csv')), header=None),
	]
	auc = [0.982263, 0.9396640, 0.9565]
	text_pos = [
		[-0.025, 1],
		[0.125, 0.725],
		[0.325, 0.9],
	]

	mpl.clf()
	for i in range(len(data)):
		mpl.plot(data[i][0], data[i][1], label=alg_name[i], c=color[i], ls=line_style[i])
		mpl.text(text_pos[i][0], text_pos[i][1], f"AUC = {auc[i]:.4f}", c=color[i])
	mpl.legend(loc='lower right')
	mpl.xlabel('False Positive Rate')
	mpl.ylabel('True Positive Rate')
	mpl.tight_layout()
	mpl.savefig(prefix / 'doc/Manuscript/img/ROC.Relational.DDoS.pdf', transparent=False)

if __name__ == '__main__':
	root = (Path(__file__) / '../../../..').resolve()
	alg_name = ['RejectMIDAS', 'MIDAS', 'SedanSpot']
	color = ['#e25a42', '#f6b656', '#7abfcc']
	line_style = ['-', '--', '-.']
	mpl.rcParams.update({
		'font.family': 'Ubuntu Condensed',
		'font.size': 14,
	})
	NormalDarpa(root)
	RelationalDarpa(root)
	NormalDDoS(root)
	RelationalDDoS(root)
