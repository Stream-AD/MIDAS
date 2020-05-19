#pragma once

#include <cmath>

#include "EdgeHash.hpp"
#include "Option.h"

namespace MIDAS::CPU {
struct NormalCore {
	int timestampCurrent = 1;
	int* const index; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	EdgeHash numCurrent, numTotal;
	const Option& option;

	NormalCore(int numRow, int numColumn, const Option& option = { }):
		index(new int[numRow]),
		numCurrent(numRow, numColumn),
		numTotal(numCurrent),
		option(option) { }

	virtual ~NormalCore() {
		delete[] index;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 || t - 1 == 0 ? 0 : pow((a - s / t) * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			numCurrent.MultiplyAll(option.ScalingCoefficient(timestamp, timestampCurrent));
			timestampCurrent = timestamp;
		}
		numCurrent.Hash(source, destination, index);
		numCurrent.Add(index);
		numTotal.Add(index);
		return ComputeScore(numCurrent(index), numTotal(index), timestamp);
	}
};
}