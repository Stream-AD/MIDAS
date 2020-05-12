#pragma once

#include <cmath>
#include <algorithm>

#include "EdgeHash.hpp"
#include "NodeHash.hpp"

namespace RejectMIDAS::CPU {
struct RejectCore {
	const float threshold;
	int timestampCurrent = 1;
	const float factor;
	int* const indexEdge; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	int* const indexSource;
	int* const indexDestination;
	EdgeHash numCurrentEdge, numTotalEdge, scoreEdge;
	NodeHash numCurrentSource, numTotalSource, scoreSource;
	NodeHash numCurrentDestination, numTotalDestination, scoreDestination;

	RejectCore(int numRow, int numColumn, float threshold, float factor = 0.5):
		threshold(threshold),
		factor(factor),
		indexEdge(new int[numRow]),
		indexSource(new int[numRow]),
		indexDestination(new int[numRow]),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge),
		numCurrentSource(numRow, numColumn),
		numTotalSource(numCurrentSource),
		scoreSource(numCurrentSource),
		numCurrentDestination(numRow, numColumn),
		numTotalDestination(numCurrentDestination),
		scoreDestination(numCurrentDestination) { }

	virtual ~RejectCore() {
		delete[] indexEdge;
		delete[] indexSource;
		delete[] indexDestination;
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
			for (int i = 0; i < numCurrentSource.lenData; i++)
				numTotalSource.data[i] += scoreSource.data[i] < threshold ?
					numCurrentSource.data[i] : timestampCurrent - 1 ?
						numTotalSource.data[i] / (timestampCurrent - 1) : 0;
			for (int i = 0; i < numCurrentDestination.lenData; i++)
				numTotalDestination.data[i] += scoreDestination.data[i] < threshold ?
					numCurrentDestination.data[i] : timestampCurrent - 1 ?
						numTotalDestination.data[i] / (timestampCurrent - 1) : 0;
			numCurrentEdge.MultiplyAll(factor);
			numCurrentSource.MultiplyAll(factor);
			numCurrentDestination.MultiplyAll(factor);
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Hash(source, destination, indexEdge);
		numCurrentEdge.Add(indexEdge);
		numCurrentSource.Hash(source, indexSource);
		numCurrentSource.Add(indexSource);
		numCurrentDestination.Hash(destination, indexDestination);
		numCurrentDestination.Add(indexDestination);
		return std::max({
			scoreEdge.Assign(indexEdge, ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp)),
			scoreSource.Assign(indexSource, ComputeScore(numCurrentSource(indexSource), numTotalSource(indexSource), timestamp)),
			scoreDestination.Assign(indexDestination, ComputeScore(numCurrentDestination(indexDestination), numTotalDestination(indexDestination), timestamp)),
		});
	}
};
}
