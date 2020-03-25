#pragma once

#include "EdgeHash.hpp"

namespace RejectMIDAS::CPU {
struct NormalCore {
	// Fields
	// --------------------------------------------------------------------------------

	const float threshold;
	EdgeHash numCurrentEdge;
	EdgeHash numTotalEdge;
	EdgeHash scoreEdge;
	int timestampCurrent = 1;
	const int lenHash;

	// Methods
	// --------------------------------------------------------------------------------

	explicit NormalCore(int numRow, int numColumn, float thresholdRejection = 1e3f) :
		lenHash(numRow * numColumn),
		threshold(thresholdRejection),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge) {
		printf("Algorithm = RejectMIDAS.NormalCore\n");
	}

	static float ComputeScore(float a, float s, float t) {
		return abs(s * (t - 1)) < 1e-4 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			for (const int i: numCurrentEdge.indexModified)
				numTotalEdge.data[i] += scoreEdge.data[i] < threshold ?
					numCurrentEdge.data[i] : timestampCurrent - 1 ?
						numTotalEdge.data[i] / (timestampCurrent - 1) : 0;
			numCurrentEdge.Clear();
			scoreEdge.Clear();
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Add(source, destination);
		return scoreEdge.Assign(source, destination, ComputeScore(numCurrentEdge(source, destination), numTotalEdge(source, destination), timestamp));
	}
};
}
