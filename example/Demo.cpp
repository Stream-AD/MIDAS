#include <cstdio>
#include <cstdlib>
#include <chrono>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"
#include "CPU/FilteringCore.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	const auto pathMeta = SOLUTION_DIR"data/DARPA/darpa_shape.txt";
	const auto pathData = SOLUTION_DIR"data/DARPA/darpa_midas.csv";
	const auto pathGroundTruth = SOLUTION_DIR"data/DARPA/darpa_ground_truth.csv";

	// Implementation
	// --------------------------------------------------------------------------------

	const unsigned seed = time(nullptr);
	printf("Seed = %u\t// In case of reproduction\n", seed);
	srand(seed); // Many rand(), need to init

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

	// Do the magic
	// Of course, I can merge loading and processing together, but this demo is also for benchmarking.

	// MIDAS::CPU::NormalCore midas(2, 1024);
	// MIDAS::CPU::RelationalCore midas(2, 1024);
	MIDAS::CPU::FilteringCore midas(2, 1024, 1e3f);
	const auto score = new float[n];
	const auto time = high_resolution_clock::now();
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);
	printf("Time = %lldms\t// Algorithm is finished\n", duration_cast<milliseconds>(high_resolution_clock::now() - time).count());

	// Write output scores

	const char* pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);
	printf("// Raw anomaly scores are exported to\n// " SOLUTION_DIR"temp/Score.txt\n");

	// Evaluate scores

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore);
	system(command);

	// Clean up

	delete[] source;
	delete[] destination;
	delete[] timestamp;
	delete[] score;
}
