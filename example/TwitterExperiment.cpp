#include <ctime>
#include <algorithm>

#include "CPU/NormalCore.hpp"
#include "CPU/RelationalCore.hpp"

int main(int argc, char* argv[]) {
	const auto pathData = SOLUTION_DIR"data/twitter_security_processed.csv";

	// --------------------------------------------------------------------------------

	srand(time(nullptr));
	const auto fileData = fopen(pathData, "r");
	const auto fileScore = fopen(SOLUTION_DIR"temp/Score.txt", "w");
	int timestampCurrent = 1; // Always starts from 1
	float scoreMost = -std::numeric_limits<float>::infinity(); // Highest
	RejectMIDAS::CPU::RelationalCore midas(2, 2719, 1e4f);
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


}