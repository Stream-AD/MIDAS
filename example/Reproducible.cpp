// -----------------------------------------------------------------------------
// Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
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

#include <chrono>
//
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
	// midas.numCurrent.param1[0] = midas.numTotal.param1[0] = 2;
	// midas.numCurrent.param1[1] = midas.numTotal.param1[1] = 3;
	// midas.numCurrent.param2[0] = midas.numTotal.param2[0] = 5;
	// midas.numCurrent.param2[1] = midas.numTotal.param2[1] = 7;
	// MIDAS::RelationalCore midas(2, 1024);
	// midas.numCurrentSource.param1[0] = midas.numTotalSource.param1[0] = 2;
	// midas.numCurrentSource.param1[1] = midas.numTotalSource.param1[1] = 3;
	// midas.numCurrentSource.param2[0] = midas.numTotalSource.param2[0] = 5;
	// midas.numCurrentSource.param2[1] = midas.numTotalSource.param2[1] = 7;
	// midas.numCurrentDestination.param1[0] = midas.numTotalDestination.param1[0] = 11;
	// midas.numCurrentDestination.param1[1] = midas.numTotalDestination.param1[1] = 13;
	// midas.numCurrentDestination.param2[0] = midas.numTotalDestination.param2[0] = 17;
	// midas.numCurrentDestination.param2[1] = midas.numTotalDestination.param2[1] = 19;
	// midas.numCurrentEdge.param1[0]  = midas.numTotalEdge.param1[0] = 23;
	// midas.numCurrentEdge.param1[1]  = midas.numTotalEdge.param1[1] = 29;
	// midas.numCurrentEdge.param2[0]  = midas.numTotalEdge.param2[0] = 31;
	// midas.numCurrentEdge.param2[1]  = midas.numTotalEdge.param2[1] = 37;
	MIDAS::FilteringCore midas(2, 1024, 1e3f);
	midas.numCurrentSource.param1[0] = midas.numTotalSource.param1[0] = midas.scoreSource.param1[0] = 2;
	midas.numCurrentSource.param1[1] = midas.numTotalSource.param1[1] = midas.scoreSource.param1[1] = 3;
	midas.numCurrentSource.param2[0] = midas.numTotalSource.param2[0] = midas.scoreSource.param2[0] = 5;
	midas.numCurrentSource.param2[1] = midas.numTotalSource.param2[1] = midas.scoreSource.param2[1] = 7;
	midas.numCurrentDestination.param1[0] = midas.numTotalDestination.param1[0] = midas.scoreDestination.param1[0] = 11;
	midas.numCurrentDestination.param1[1] = midas.numTotalDestination.param1[1] = midas.scoreDestination.param1[1] = 13;
	midas.numCurrentDestination.param2[0] = midas.numTotalDestination.param2[0] = midas.scoreDestination.param2[0] = 17;
	midas.numCurrentDestination.param2[1] = midas.numTotalDestination.param2[1] = midas.scoreDestination.param2[1] = 19;
	midas.numCurrentEdge.param1[0] = midas.numTotalEdge.param1[0] = midas.scoreEdge.param1[0] = 23;
	midas.numCurrentEdge.param1[1] = midas.numTotalEdge.param1[1] = midas.scoreEdge.param1[1] = 29;
	midas.numCurrentEdge.param2[0] = midas.numTotalEdge.param2[0] = midas.scoreEdge.param2[0] = 31;
	midas.numCurrentEdge.param2[1] = midas.numTotalEdge.param2[1] = midas.scoreEdge.param2[1] = 37;

	const auto score = new float[n];
	const auto time = high_resolution_clock::now();
	for (int i = 0; i < n; i++)
		score[i] = midas(source[i], destination[i], timestamp[i]);
	printf("Time = %lldms\t// Algorithm is finished\n", duration_cast<milliseconds>(high_resolution_clock::now() - time).count());

	// Write output scores
	// --------------------------------------------------------------------------------

	const auto pathScore = SOLUTION_DIR"temp/Score.txt";
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
