#include <cstdio>
#include <ctime>
#include <cstdlib>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	// const auto pathMeta = SOLUTION_DIR"data/darpa_shape.txt";
	// const auto pathData = SOLUTION_DIR"data/darpa_processed.csv";
	// const auto pathGroundTruth = SOLUTION_DIR"data/darpa_ground_truth.csv";

	// const auto pathMeta = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_shape.txt";
	// const auto pathData = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_processed.csv";
	// const auto pathGroundTruth = SOLUTION_DIR"data/DDoS/Balanced/final_dataset_ground_truth.csv";

	const auto pathMeta = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_shape.txt";
	const auto pathData = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_processed.csv";
	const auto pathGroundTruth = SOLUTION_DIR"data/DDoS/Unbalanced/unbalaced_20_80_dataset_ground_truth.csv";

	// Implementation
	// --------------------------------------------------------------------------------

	const unsigned seed = time(nullptr);
	printf("Seed = %u\t// In case of reproduction\n", seed);
	srand(seed); // Many rand(), need to init

	// Read meta (total number of records)
	// PreprocessData.py will generate those meta files

	#ifndef NDEBUG
	const int n = 100000;
	#else
	const auto fileMeta = fopen(pathMeta, "r");
	int n;
	fscanf(fileMeta, "%d", &n);
	fclose(fileMeta);
	#endif

	// Read dataset

	const auto fileData = fopen(pathData, "r");
	const auto source = new int[n];
	const auto destination = new int[n];
	const auto timestamp = new int[n];
	for (int i = 0; i < n; i++)
		fscanf(fileData, "%d,%d,%d", &source[i], &destination[i], &timestamp[i]);
	fclose(fileData);
	printf("# Records = %d\t// Dataset is loaded\n", n);

	// Do the magic
	// Of course, I can merge loading and processing together, but this demo is also for benchmarking.

	// RejectMIDAS::CPU::NormalCore midas(2, 1024, 1e1f);
	RejectMIDAS::CPU::RelationalCore midas(2, 1024, 1e2f);
	const auto score = new float[n];
	const auto time = clock();
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);
	printf("Time = %ldms\t// Algorithm is finished\n", (clock() - time) * 1000 / CLOCKS_PER_SEC);

	// Write output scores

	#ifdef NDEBUG
	const char* pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);
	printf("// Exporting score is completed\n");

	// Evaluate scores

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore);
	system(command);
	#endif

	// Clean up

	delete[] source;
	delete[] destination;
	delete[] timestamp;
	delete[] score;
}
