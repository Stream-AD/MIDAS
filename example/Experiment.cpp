#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <initializer_list>
#include <random>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"

void TestAUC(const int n, const char pathGroundTruth[], const std::initializer_list<int>& numColumn, const std::initializer_list<float>& thresholdRejection, const int numRepeat, const int source[], const int destination[], const int timestamp[]) {
	const auto pathExperimentResult = SOLUTION_DIR"temp/Experiment.csv";
	const auto fileExperimentResult = fopen(pathExperimentResult, "w");
	fprintf(fileExperimentResult, "numColumn,threshold,seed,auc\n");

	for (const auto column: numColumn)
		for (const auto threshold: thresholdRejection) {
			const auto seed = new int[numRepeat];
			const auto auc = new float[numRepeat];

			for (int i = 0; i < numRepeat; i++)
				seed[i] = rand();

			#pragma omp parallel for schedule(static)
			for (int indexRun = 0; indexRun < numRepeat; indexRun++) {
				srand(seed[indexRun]); // Re-seed, I don't want all results to be the same

				const auto score = new float[n];
				RejectMIDAS::CPU::RelationalCore midas(2, column, threshold);
				for (int i = 0; i < n; i++)
					score[i] = midas(source[i], destination[i], timestamp[i]);

				char pathScore[1024];
				sprintf(pathScore, SOLUTION_DIR"temp/Score%02d.txt", indexRun);
				const auto fileScore = fopen(pathScore, "w");
				for (int i = 0; i < n; i++)
					fprintf(fileScore, "%f\n", score[i]);
				fclose(fileScore);

				char command[1024];
				sprintf(command, "python %s %s %s %02d", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore, indexRun);
				system(command);

				char pathAUC[1024];
				sprintf(pathAUC, SOLUTION_DIR"temp/AUC%02d.txt", indexRun);
				const auto fileAUC = fopen(pathAUC, "r");
				fscanf(fileAUC, "%f", auc + indexRun);
				fclose(fileAUC);
			}

			for (int i = 0; i < numRepeat; i++)
				fprintf(fileExperimentResult, "%d,%g,%d,%f\n", column, threshold, seed[i], auc[i]);

			printf("// Above results use column = %d, threshold = %g\n", column, threshold);

			delete[] seed;
			delete[] auc;
		}

	fclose(fileExperimentResult);
}

void TestSpeed(const int n, const char pathGroundTruth[], const std::initializer_list<int>& numColumn, const std::initializer_list<float>& thresholdRejection, const int numRepeat, const int source[], const int destination[], const int timestamp[]) {
	const auto pathExperimentResult = SOLUTION_DIR"temp/Experiment.csv";
	const auto fileExperimentResult = fopen(pathExperimentResult, "w");
	fprintf(fileExperimentResult, "numColumn,threshold,seed,time\n");

	for (const auto column: numColumn)
		for (const auto threshold: thresholdRejection) {
			const auto seed = new int[numRepeat];
			const auto time = new int[numRepeat];
			const auto score = new float[n];

			for (int indexRun = 0; indexRun < numRepeat; indexRun++) {
				srand(seed[indexRun] = rand()); // Re-seed, I don't want all results to be the same
				RejectMIDAS::CPU::RelationalCore midas(2, column, threshold);
				const auto timeBegin = clock();
				for (int i = 0; i < n; i++)
					score[i] = midas(source[i], destination[i], timestamp[i]);
				time[indexRun] = clock() - timeBegin;
				printf("Time%02d = %dms\n", indexRun, time[indexRun]);
			}

			char pathScore[1024];
			sprintf(pathScore, SOLUTION_DIR"temp/Score.txt");
			const auto fileScore = fopen(pathScore, "w");
			for (int i = 0; i < n; i++)
				fprintf(fileScore, "%f\n", score[i]);
			fclose(fileScore);

			char command[1024];
			sprintf(command, "python %s %s %s", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore);
			printf("// Below is the AUC of the last run\n");
			system(command);

			for (int i = 0; i < numRepeat; i++)
				fprintf(fileExperimentResult, "%d,%g,%d,%d\n", column, threshold, seed[i], time[i]);

			printf("// Above results use column = %d, threshold = %g\n", column, threshold);

			delete[] seed;
			delete[] time;
			delete[] score;
		}

	fclose(fileExperimentResult);
}

void ReproduceROC(const int n, const char pathGroundTruth[], const int numColumn, const float thresholdRejection, const int seed, const int source[], const int destination[], const int timestamp[]) {
	srand(seed);

	const auto score = new float[n];
	RejectMIDAS::CPU::RelationalCore midas(2, numColumn, thresholdRejection);
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);

	const auto pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);

	printf("// Python is generating ROC curve\n");

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/ReproduceROC.py", pathGroundTruth, pathScore);
	system(command);

	delete[] score;
}

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	// const auto pathMeta = SOLUTION_DIR"data/darpa_shape.txt";
	// const auto pathData = SOLUTION_DIR"data/darpa_processed.csv";
	// const auto pathGroundTruth = SOLUTION_DIR"data/darpa_ground_truth.csv";

	const auto pathMeta = SOLUTION_DIR"data/final_dataset_shape.txt";
	const auto pathData = SOLUTION_DIR"data/final_dataset_processed.csv";
	const auto pathGroundTruth = SOLUTION_DIR"data/final_dataset_ground_truth.csv";

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
	const auto numColumn = {1024};
	const auto thresholdRejection = {1e0f, 1e1f, 1e2f, 1e3f, 1e4f, 1e5f, 1e6f, 1e7f};
	TestAUC(n, pathGroundTruth, numColumn, thresholdRejection, numRepeat, source, destination, timestamp);
	// TestSpeed(n, pathGroundTruth, numColumn, thresholdRejection, numRepeat, source, destination, timestamp);
	// ReproduceROC(n, pathGroundTruth, 1024, 1000, 18223, source, destination, timestamp);

	// Clean up

	char command[1024];
	sprintf(command, "python %s %s", SOLUTION_DIR"util/DeleteTempFile.py", "Score*.txt AUC*.txt");
	system(command);

	delete[] source;
	delete[] destination;
	delete[] timestamp;
}
