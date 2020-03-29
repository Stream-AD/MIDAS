#pragma once

#include <cmath>

#include "EdgeHash.hpp"

namespace RejectMIDAS::CPU {
struct NormalCore {
	// Fields
	// --------------------------------------------------------------------------------

	const float threshold;
	int timestampCurrent = 1;
	int* const indexEdge;
	EdgeHash numCurrentEdge;
	EdgeHash numTotalEdge;
	EdgeHash scoreEdge;

	// Methods
	// --------------------------------------------------------------------------------

	NormalCore(int numRow, int numColumn, float thresholdRejection):
		threshold(thresholdRejection),
		indexEdge(new int[numRow]),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge) { }

	virtual ~NormalCore() {
		delete[] indexEdge;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			for (int i = 0; i < numCurrentEdge.lenData; i++)
				numTotalEdge.data[i] += scoreEdge.data[i] < threshold ?
					numCurrentEdge.data[i] : timestampCurrent - 1 ?
						numTotalEdge.data[i] / (timestampCurrent - 1) : 0;
			numCurrentEdge.Clear();
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Hash(source, destination, indexEdge);
		numCurrentEdge.Add(indexEdge);
		return scoreEdge.Assign(indexEdge, ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp));
	}
};
}
