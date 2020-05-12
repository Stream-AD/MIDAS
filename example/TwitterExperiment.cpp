#include <ctime>
#include <algorithm>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.h"
#include "CPU/RejectCore.hpp"

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
	// RejectMIDAS::CPU::NormalCore midas(2, 1024);
	// RejectMIDAS::CPU::RelationalCore midas(2, 1024);
	RejectMIDAS::CPU::RejectCore midas(2, 1024, 1e3f);
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