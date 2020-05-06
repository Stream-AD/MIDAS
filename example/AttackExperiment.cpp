#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>

#include <tbb/parallel_for.h>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"

void ThresholdVsAUC(int n, const char pathGroundTruth[], int numColumn, const std::vector<float>& thresholds, int numRepeat, const int source[], const int destination[], const int timestamp[]) {
	/**
	 * If threshold is 0, then all edges will be rejected, and all edges will get 0 score.
	 * */

	const auto seed = new int[numRepeat];
	const auto auc = new float[thresholds.size() * numRepeat];
	std::for_each(seed, seed + numRepeat, [](int& a) { a = rand(); });

	tbb::parallel_for<int>(0, thresholds.size(), [&](int i) {
		tbb::parallel_for(0, numRepeat, [&](int j) {
			srand(seed[j]);

			char pathScore[260];
			sprintf(pathScore, SOLUTION_DIR"temp/Score%03d.txt", i * numRepeat + j);
			const auto fileScore = fopen(pathScore, "w");
			RejectMIDAS::CPU::NormalCore midas(2, numColumn, thresholds[i]);
			// RejectMIDAS::CPU::RelationalCore midas(2, numColumn, thresholds[i]);
			for (int k = 0; k < n; k++)
				fprintf(fileScore, "%f\n", midas(source[k], destination[k], timestamp[k]));
			fclose(fileScore);

			char command[1024];
			sprintf(command, "python %s %s %s %d", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore, i * numRepeat + j);
			system(command);

			char pathAUC[260];
			sprintf(pathAUC, SOLUTION_DIR"temp/AUC%d.txt", i * numRepeat + j);
			const auto fileAUC = fopen(pathAUC, "r");
			fscanf(fileAUC, "%f", auc + i * numRepeat + j);
			fclose(fileAUC);
		});
	});

	const auto fileResult = fopen(SOLUTION_DIR"temp/Experiment.csv", "w");
	fprintf(fileResult, "numColumn,threshold,seed,auc\n");
	for (int i = 0; i < thresholds.size(); i++)
		for (int j = 0; j < numRepeat; j++)
			fprintf(fileResult, "%d,%g,%d,%f\n", numColumn, thresholds[i], seed[j], auc[i * numRepeat + j]);
	fclose(fileResult);

	delete[] seed;
	delete[] auc;
}

void ThresholdVsTime(int n, int numColumn, const std::vector<float>& thresholds, int numRepeat, const int* source, const int* destination, const int* timestamp) {
	const auto time = new int[thresholds.size() * numRepeat];
	const auto seed = new int[numRepeat];
	std::for_each(seed, seed + numRepeat, [](int& a) { a = rand(); });
	for (int i = 0; i < thresholds.size(); i++) {
		for (int j = 0; j < numRepeat; j++) {
			srand(seed[j]);
			RejectMIDAS::CPU::NormalCore midas(2, numColumn, thresholds[i]);
			// RejectMIDAS::CPU::RelationalCore midas(2, numColumn, thresholds[i]);
			const auto timeBegin = clock();
			for (int k = 0; k < n; k++)
				midas(source[k], destination[k], timestamp[k]);
			printf("Time%02d = %dms\n", j, time[i * numRepeat + j] = (clock() - timeBegin) * 1000 / CLOCKS_PER_SEC);
		}
		printf("// Above results use threshold = %g\n", thresholds[i]);
	}
	const auto fileExperimentResult = fopen(SOLUTION_DIR"temp/Experiment.csv", "w");
	fprintf(fileExperimentResult, "numColumn,threshold,seed,time\n");
	for (int i = 0; i < thresholds.size(); i++)
		for (int j = 0; j < numRepeat; j++)
			fprintf(fileExperimentResult, "%d,%g,%d,%d\n", numColumn, thresholds[i], seed[j], time[i * numRepeat + j]);
	fclose(fileExperimentResult);
	delete[] time;
	delete[] seed;
}

void ReproduceROC(int n, const char pathGroundTruth[], int numColumn, float threshold, int seed, const int source[], const int destination[], const int timestamp[]) {
	srand(seed);

	const auto score = new float[n];
	RejectMIDAS::CPU::NormalCore midas(2, numColumn, threshold);
	// RejectMIDAS::CPU::RelationalCore midas(2, numColumn, threshold);
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);

	const auto pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);

	printf("// Reproduction is done, python is generating the ROC curve\n");

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/ReproduceROC.py", pathGroundTruth, pathScore);
	system(command);

	delete[] score;
}

void NumRecordVsTime(int numColumn, float threshold, const std::vector<int>& numsRecord, int numRepeat, const int* source, const int* destination, const int* timestamp) {
	const auto time = new long long[numsRecord.size() * numRepeat];
	const auto seed = new int[numRepeat];
	std::for_each(seed, seed + numRepeat, [](int& a) { a = rand(); });
	for (int i = 0; i < numsRecord.size(); i++) {
		for (int j = 0; j < numRepeat; j++) {
			srand(seed[j]);
			// RejectMIDAS::CPU::NormalCore midas(2, numColumn, threshold);
			RejectMIDAS::CPU::RelationalCore midas(2, numColumn, threshold);
			const auto timeBegin = std::chrono::high_resolution_clock::now();
			for (int k = 0; k < numsRecord[i]; k++)
				midas(source[k], destination[k], timestamp[k]);
			printf("Time%02d = %lldus\n", j, time[i * numRepeat + j] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timeBegin).count());
		}
		printf("// Above results use numRecord = %d\n", numsRecord[i]);
	}
	const auto fileExperimentResult = fopen(SOLUTION_DIR"temp/Experiment.csv", "w");
	fprintf(fileExperimentResult, "numColumn,threshold,numRecord,seed,time\n"); // Microsecond (us), other tests are millisecond (ms)
	for (int i = 0; i < numsRecord.size(); i++)
		for (int j = 0; j < numRepeat; j++)
			fprintf(fileExperimentResult, "%d,%g,%d,%d,%lld\n", numColumn, threshold, numsRecord[i], seed[j], time[i * numRepeat + j]);
	fclose(fileExperimentResult);
	delete[] time;
	delete[] seed;
}

void NumColumnVsTime(int n, const std::vector<int>& numsColumn, float threshold, int numRepeat, const int* source, const int* destination, const int* timestamp) {
	const auto time = new long long[numsColumn.size() * numRepeat];
	const auto seed = new int[numRepeat];
	std::for_each(seed, seed + numRepeat, [](int& a) { a = rand(); });
	for (int i = 0; i < numsColumn.size(); i++) {
		for (int j = 0; j < numRepeat; j++) {
			srand(seed[j]);
			// RejectMIDAS::CPU::NormalCore midas(2, numsColumn[i], threshold);
			RejectMIDAS::CPU::RelationalCore midas(2, numsColumn[i], threshold);
			const auto timeBegin = std::chrono::high_resolution_clock::now();
			for (int k = 0; k < n; k++)
				midas(source[k], destination[k], timestamp[k]);
			printf("Time%02d = %lldus\n", j, time[i * numRepeat + j] = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - timeBegin).count());
		}
		printf("// Above results use numColumn = %d\n", numsColumn[i]);
	}
	const auto fileExperimentResult = fopen(SOLUTION_DIR"temp/Experiment.csv", "w");
	fprintf(fileExperimentResult, "numColumn,threshold,seed,time\n"); // Microsecond (us), other tests are millisecond (ms)
	for (int i = 0; i < numsColumn.size(); i++)
		for (int j = 0; j < numRepeat; j++)
			fprintf(fileExperimentResult, "%d,%g,%d,%lld\n", numsColumn[i], threshold, seed[j], time[i * numRepeat + j]);
	fclose(fileExperimentResult);
	delete[] time;
	delete[] seed;
}

void FactorVsAUC(int n, const char* pathGroundTruth, int numColumn, float threshold, const std::vector<float>& factors, int numRepeat, const int* source, const int* destination, const int* timestamp) {
	/**
	 * This experiment only repeats 11 time due to the large number of factors
	 * */

	const auto seed = new int[numRepeat];
	const auto auc = new float[factors.size() * numRepeat];
	std::for_each(seed, seed + numRepeat, [](int& a) { a = rand(); });

	tbb::parallel_for<int>(0, factors.size(), [&](int i) {
		tbb::parallel_for(0, numRepeat, [&](int j) {
			srand(seed[j]);

			char pathScore[260];
			sprintf(pathScore, SOLUTION_DIR"temp/Score%03d.txt", i * numRepeat + j);
			const auto fileScore = fopen(pathScore, "w");
			// RejectMIDAS::CPU::NormalCore midas(2, numColumn, threshold, factors[i]);
			RejectMIDAS::CPU::RelationalCore midas(2, numColumn, threshold, factors[i]);
			for (int k = 0; k < n; k++)
				fprintf(fileScore, "%f\n", midas(source[k], destination[k], timestamp[k]));
			fclose(fileScore);

			char command[1024];
			sprintf(command, "python %s %s %s %d", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore, i * numRepeat + j);
			system(command);

			char pathAUC[260];
			sprintf(pathAUC, SOLUTION_DIR"temp/AUC%d.txt", i * numRepeat + j);
			const auto fileAUC = fopen(pathAUC, "r");
			fscanf(fileAUC, "%f", auc + i * numRepeat + j);
			fclose(fileAUC);
		});
	});

	const auto fileResult = fopen(SOLUTION_DIR"temp/Experiment.csv", "w");
	fprintf(fileResult, "numColumn,threshold,factor,seed,auc\n");
	for (int i = 0; i < factors.size(); i++)
		for (int j = 0; j < numRepeat; j++)
			fprintf(fileResult, "%d,%g,%g,%d,%f\n", numColumn, threshold, factors[i], seed[j], auc[i * numRepeat + j]);
	fclose(fileResult);

	delete[] seed;
	delete[] auc;
}

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	const auto pathMeta = SOLUTION_DIR"data/DARPA/darpa_shape.txt";
	const auto pathData = SOLUTION_DIR"data/DARPA/darpa_processed.csv";
	const auto pathGroundTruth = SOLUTION_DIR"data/DARPA/darpa_ground_truth.csv";

	// const auto pathMeta = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_shape.txt";
	// const auto pathData = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_processed.csv";
	// const auto pathGroundTruth = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_ground_truth.csv";

	// const auto pathMeta = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_shape.txt";
	// const auto pathData = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_processed.csv";
	// const auto pathGroundTruth = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_ground_truth.csv";

	// Implementation
	// --------------------------------------------------------------------------------

	srand(time(nullptr));

	// Read meta (total number of records)
	// PreprocessData.py will generate those meta files

	const auto fileMeta = fopen(pathMeta, "r");
	int n;
	fscanf(fileMeta, "%d", &n);
	fclose(fileMeta);

	// Read dataset

	const auto fileData = fopen(pathData, "r");
	const auto source = new int[n];
	const auto destination = new int[n];
	const auto timestamp = new int[n];
	for (int i = 0; i < n; i++)
		fscanf(fileData, "%d,%d,%d", &source[i], &destination[i], &timestamp[i]);
	fclose(fileData);
	printf("# Records = %d\t// Dataset is loaded\n", n);

	// Call runner

	const int numRepeat = 21;
	const auto numColumn = 1024;

	const auto thresholds = {1e0f, 1e1f, 1e2f, 1e3f, 1e4f, 1e5f, 1e6f, 1e7f};
	// ThresholdVsAUC(n, pathGroundTruth, numColumn, thresholds, numRepeat, source, destination, timestamp);
	// ThresholdVsTime(n, numColumn, thresholds, numRepeat, source, destination, timestamp);
	// ReproduceROC(n, pathGroundTruth, numColumn, 1000, 8918, source, destination, timestamp);

	const auto factors = {0.0f, 0.2f, 0.4f, 0.6f, 0.8f, 0.9f, 0.99f, 0.992f, 0.994f, 0.996f, 0.998f, 0.999f, 0.9992f, 0.9994f, 0.9996f, 0.9998f, 1.0f};
	FactorVsAUC(n, pathGroundTruth, numColumn, 1e3f, factors, numRepeat, source, destination, timestamp);

	const auto numsRecord = {1 << 10, 1 << 11, 1 << 12, 1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, 1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23};
	// NumRecordVsTime(numColumn, 10000, numsRecord, numRepeat, source, destination, timestamp);

	const auto numsColumn = {600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000};
	// NumColumnVsTime(n, numsColumn, 10000, numRepeat, source, destination, timestamp);

	// Clean up

	char command[1024];
	sprintf(command, "python %s %s", SOLUTION_DIR"util/DeleteTempFile.py", "Score*.txt AUC*.txt");
	system(command);

	delete[] source;
	delete[] destination;
	delete[] timestamp;
}
