#include <chrono>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	// const char* pathMeta = SOLUTION_DIR"data/darpa_shape.txt";
	// const char* pathData = SOLUTION_DIR"data/darpa_processed.csv";

	const char* pathMeta = SOLUTION_DIR"data/final_dataset_shape.txt";
	const char* pathData = SOLUTION_DIR"data/final_dataset_processed.csv";

	// Implementation
	// --------------------------------------------------------------------------------

	// Read meta (total number of records)
	// PreprocessData.py will generate those meta files

#ifndef NDEBUG
	const int n = 10000;
#else
	FILE* fileMeta;
	fopen_s(&fileMeta, pathMeta, "r");
	int n;
	fscanf_s(fileMeta, "%d", &n);
	fclose(fileMeta);
#endif

	// Read dataset

	FILE* fileData;
	fopen_s(&fileData, pathData, "r");
	const auto source = new int[n];
	const auto destination = new int[n];
	const auto timestamp = new int[n];
	for (int i = 0; i < n; i++)
		fscanf_s(fileData, "%d,%d,%d,%*d", &source[i], &destination[i], &timestamp[i]);
	fclose(fileData);
	printf("# Records = %d\t// Dataset is loaded\n", n);

	// Do the magic
	// Of course, I can merge loading and processing together, but this demo is also for benchmarking.

	// RejectMIDAS::CPU::NormalCore midas(2, 1024, 1e-3f);
	RejectMIDAS::CPU::RelationalCore midas(2, 1024, 1e-3f);
	const auto score = new float[n];
	const auto time = high_resolution_clock::now().time_since_epoch();
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);
	printf("Time = %lldms\t// Algorithm is finished\n", duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch() - time).count());

	// Write output scores

	const char* pathScore = SOLUTION_DIR"temp/score.txt";
	FILE* fileScore;
	fopen_s(&fileScore, pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);
	printf("// Exporting score is completed\n");

	// Evaluate scores

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/EvaluateScore.py", pathData, pathScore);
	system(command);

	// Clean up

	delete[] source;
	delete[] destination;
	delete[] timestamp;
 	delete[] score;
}
