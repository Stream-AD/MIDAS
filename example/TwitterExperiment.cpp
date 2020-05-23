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

#include <ctime>
#include <algorithm>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"
#include "CPU/FilteringCore.hpp"

int main(int argc, char* argv[]) {
	const auto pathData = SOLUTION_DIR"data/TwitterSecurity/twitter_security_processed.csv";
	const auto pathEvent = SOLUTION_DIR"data/TwitterSecurity/twitter_security_ground_truth.csv";

	// --------------------------------------------------------------------------------

	srand(time(nullptr));
	const auto fileData = fopen(pathData, "r");
	const auto pathScore = SOLUTION_DIR"temp/Score.txt";
	const auto fileScore = fopen(pathScore, "w");
	int timestampCurrent = 1; // Always starts from 1
	float scoreMost = -std::numeric_limits<float>::infinity(); // Highest
	// MIDAS::CPU::NormalCore midas(2, 1024);
	// MIDAS::CPU::RelationalCore midas(2, 1024);
	MIDAS::CPU::FilteringCore midas(2, 1024, 1e3f);
	int source, destination, timestamp;
	while (~fscanf(fileData, "%d,%d,%d", &timestamp, &source, &destination)) {
		if (timestamp > timestampCurrent) {
			fprintf(fileScore, "%f\n", scoreMost);
			scoreMost = -std::numeric_limits<float>::infinity();
			timestampCurrent = timestamp;
		}
		scoreMost = std::max(scoreMost, midas(source, destination, timestamp));
	}
	fprintf(fileScore, "%f\n", scoreMost);
	fclose(fileData);
	fclose(fileScore);

	char command[1024];
	sprintf(command, "python %s %s %s", SOLUTION_DIR"util/PlotAnomalousEvent.py", pathScore, pathEvent);
	system(command);
}