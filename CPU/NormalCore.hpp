#pragma once

#include <cmath>

#include "EdgeHash.hpp"

namespace RejectMIDAS::CPU {
struct NormalCore {
	const float threshold;
	int timestampCurrent = 1;
	const float factor;
	int* const indexEdge;
	EdgeHash numCurrentEdge, numTotalEdge, scoreEdge;

	NormalCore(int numRow, int numColumn, float threshold, float factor = 0.5):
		threshold(threshold),
		factor(factor),
		indexEdge(new int[numRow]),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge) { }

	virtual ~NormalCore() {
		delete[] indexEdge;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1)); // If t == 1, then s == 0, so no need to check twice
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			for (int i = 0; i < numCurrentEdge.lenData; i++)
				numTotalEdge.data[i] += scoreEdge.data[i] < threshold ?
					numCurrentEdge.data[i] : timestampCurrent - 1 ?
						numTotalEdge.data[i] / (timestampCurrent - 1) : 0;
			numCurrentEdge.MultiplyAll(factor);
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Hash(source, destination, indexEdge);
		numCurrentEdge.Add(indexEdge);
		return scoreEdge.Assign(indexEdge, ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp));
	}
};
}
