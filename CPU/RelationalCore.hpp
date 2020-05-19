#pragma once

#include <cmath>
#include <algorithm>

#include "EdgeHash.hpp"
#include "NodeHash.hpp"

namespace MIDAS::CPU {
struct RelationalCore {
	int timestampCurrent = 1;
	const float factor;
	int* const indexEdge; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	int* const indexSource;
	int* const indexDestination;
	EdgeHash numCurrentEdge, numTotalEdge;
	NodeHash numCurrentSource, numTotalSource;
	NodeHash numCurrentDestination, numTotalDestination;

	RelationalCore(int numRow, int numColumn, float factor = 0.5):
		factor(factor),
		indexEdge(new int[numRow]),
		indexSource(new int[numRow]),
		indexDestination(new int[numRow]),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		numCurrentSource(numRow, numColumn),
		numTotalSource(numCurrentSource),
		numCurrentDestination(numRow, numColumn),
		numTotalDestination(numCurrentDestination) { }

	virtual ~RelationalCore() {
		delete[] indexEdge;
		delete[] indexSource;
		delete[] indexDestination;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 || t - 1 == 0 ? 0 : pow((a - s / t) * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			numCurrentEdge.MultiplyAll(factor);
			numCurrentSource.MultiplyAll(factor);
			numCurrentDestination.MultiplyAll(factor);
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Hash(source, destination, indexEdge);
		numCurrentEdge.Add(indexEdge);
		numTotalEdge.Add(indexEdge);
		numCurrentSource.Hash(source, indexSource);
		numCurrentSource.Add(indexSource);
		numTotalSource.Add(indexSource);
		numCurrentDestination.Hash(destination, indexDestination);
		numCurrentDestination.Add(indexDestination);
		numTotalDestination.Add(indexDestination);
		return std::max({
			ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp),
			ComputeScore(numCurrentSource(indexSource), numTotalSource(indexSource), timestamp),
			ComputeScore(numCurrentDestination(indexDestination), numTotalDestination(indexDestination), timestamp),
		});
	}
};
}