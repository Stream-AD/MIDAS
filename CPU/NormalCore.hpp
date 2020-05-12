#pragma once

#include <algorithm>

#include "EdgeHash.hpp"

namespace RejectMIDAS::CPU {
struct NormalCore {
	int timestampCurrent = 1;
	int* const index; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	EdgeHash numCurrent, numTotal;

	NormalCore(int numRow, int numColumn):
		index(new int[numRow]),
		numCurrent(numRow, numColumn),
		numTotal(numCurrent) { }

	virtual ~NormalCore() {
		delete[] index;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 ? 0 : pow((a - s / t) * t, 2) / (s * (t - 1)); // If t == 1, then s == 0, so no need to check twice
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			numCurrent.MultiplyAll(0);
			timestampCurrent = timestamp;
		}
		numCurrent.Hash(source, destination, index);
		numCurrent.Add(index);
		numTotal.Add(index);
		return ComputeScore(numCurrent(index), numTotal(index), timestamp);
	}
};
}