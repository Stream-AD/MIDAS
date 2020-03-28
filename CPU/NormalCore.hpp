#pragma once

#include <cstdio>
#include <cmath>

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
	int* const indexEdge;

	// Methods
	// --------------------------------------------------------------------------------

	explicit NormalCore(int numRow, int numColumn, float thresholdRejection = 1e3f):
		lenHash(numRow * numColumn),
		threshold(thresholdRejection),
		indexEdge(new int[numRow]),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge) {
		printf("Algorithm = RejectMIDAS.NormalCore\n");
	}

	virtual ~NormalCore() {
		delete[] indexEdge;
	}

	static float ComputeScore(float a, float s, float t) {
		return t > 1 && s > 0 ? pow(a + s - a * t, 2) / (s * (t - 1)) : 0;
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			for (int i = 0; i < numCurrentEdge.c; i++)
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
