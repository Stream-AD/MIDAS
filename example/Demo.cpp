// -----------------------------------------------------------------------------
// Copyright 2020 Rui LIU
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -----------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <chrono>

#include "NormalCore.hpp"
#include "RelationalCore.hpp"
#include "FilteringCore.hpp"

using namespace std::chrono;

int main(int argc, char* argv[]) {
	// Parameter
	// --------------------------------------------------------------------------------

	const auto pathMeta = SOLUTION_DIR"data/DARPA/darpa_shape.txt";
	const auto pathData = SOLUTION_DIR"data/DARPA/darpa_processed.csv";
	const auto pathGroundTruth = SOLUTION_DIR"data/DARPA/darpa_ground_truth.csv";

	// Random seed
	// --------------------------------------------------------------------------------

	const unsigned seed = time(nullptr);
	printf("Seed = %u\t// In case of reproduction\n", seed);
	srand(seed); // Many rand(), need to init

	// Read meta (total number of records)
	// --------------------------------------------------------------------------------
	// PreprocessData.py will generate those meta files

	const auto fileMeta = fopen(pathMeta, "r");
	int n;
	fscanf(fileMeta, "%d", &n);
	fclose(fileMeta);

	// Read dataset
	// --------------------------------------------------------------------------------

	const auto fileData = fopen(pathData, "r");
	const auto source = new int[n];
	const auto destination = new int[n];
	const auto timestamp = new int[n];
	for (int i = 0; i < n; i++)
		fscanf(fileData, "%d,%d,%d", &source[i], &destination[i], &timestamp[i]);
	fclose(fileData);
	printf("# Records = %d\t// Dataset is loaded\n", n);

	// Do the magic
	// --------------------------------------------------------------------------------
	// Of course, I can merge loading and processing together, but this demo is also for benchmarking.

	// MIDAS::NormalCore midas(2, 1024);
	// MIDAS::RelationalCore midas(2, 1024);
	MIDAS::FilteringCore midas(2, 1024, 1e3f);
	const auto score = new float[n];
	const auto time = high_resolution_clock::now();
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);
	printf("Time = %lldms\t// Algorithm is finished\n", duration_cast<milliseconds>(high_resolution_clock::now() - time).count());

	// Write output scores
	// --------------------------------------------------------------------------------

	const char* pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	for (int i = 0; i < n; i++)
		fprintf(fileScore, "%f\n", score[i]);
	fclose(fileScore);
	printf("// Raw anomaly scores are exported to\n// " SOLUTION_DIR"temp/Score.txt\n");

	// Evaluate scores
	// --------------------------------------------------------------------------------

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/EvaluateScore.py", pathGroundTruth, pathScore);
	system(command);

	// Clean up
	// --------------------------------------------------------------------------------

	delete[] source;
	delete[] destination;
	delete[] timestamp;
	delete[] score;
}
