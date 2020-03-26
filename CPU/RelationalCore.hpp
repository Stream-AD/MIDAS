#pragma once

#include <algorithm>

#include "EdgeHash.hpp"
#include "NodeHash.hpp"

namespace RejectMIDAS::CPU {
struct RelationalCore {
	// Fields
	// --------------------------------------------------------------------------------

	const float threshold;
	EdgeHash numCurrentEdge, numTotalEdge, scoreEdge;
	NodeHash numCurrentSource, numTotalSource, scoreSource;
	NodeHash numCurrentDestination, numTotalDestination, scoreDestination;
	int timestampCurrent = 1;
	const int lenHash;
	const float factor;
	int* const indexEdge; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	int* const indexSource;
	int* const indexDestination;

	// Methods
	// --------------------------------------------------------------------------------

	explicit RelationalCore(int numRow, int numColumn, float thresholdRejection = 1e3f, float factorHashShrink = 0.5f) :
		factor(factorHashShrink),
		lenHash(numRow * numColumn),
		threshold(thresholdRejection),
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
		scoreDestination(numCurrentDestination) {
		printf("Algorithm = RejectMIDAS.RelationalCore\n");
	}

	virtual ~RelationalCore() {
		delete[] indexEdge;
		delete[] indexSource;
		delete[] indexDestination;
	}

	static float ComputeScore(float a, float s, float t) {
		return abs(s * (t - 1)) < 1e-4 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
			#pragma omp for
			for (int i = 0; i < numCurrentEdge.c; i++)
				numTotalEdge.data[i] += scoreEdge.data[i] < threshold ?
					numCurrentEdge.data[i] : timestampCurrent - 1 ?
						numTotalEdge.data[i] / (timestampCurrent - 1) : 0;
			#pragma omp for
			for (int i = 0; i < numCurrentSource.c; i++)
				numTotalSource.data[i] += scoreSource.data[i] < threshold ?
					numCurrentSource.data[i] : timestampCurrent - 1 ?
						numTotalSource.data[i] / (timestampCurrent - 1) : 0;
			#pragma omp for
			for (int i = 0; i < numCurrentDestination.c; i++)
				numTotalDestination.data[i] += scoreDestination.data[i] < threshold ?
					numCurrentDestination.data[i] : timestampCurrent - 1 ?
						numTotalDestination.data[i] / (timestampCurrent - 1) : 0;
			numCurrentEdge.MultiplyAll(factor);
			numCurrentSource.MultiplyAll(factor);
			numCurrentDestination.MultiplyAll(factor);
			scoreEdge.Clear();
			scoreSource.Clear();
			scoreDestination.Clear();
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Hash(source, destination, indexEdge);
		numCurrentSource.Hash(source, indexSource);
		numCurrentDestination.Hash(destination, indexDestination);
		numCurrentEdge.Add(indexEdge);
		numCurrentSource.Add(indexSource);
		numCurrentDestination.Add(indexDestination);
		return std::log(1 + std::max({
			scoreEdge.Assign(indexEdge, ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp)),
			scoreSource.Assign(indexSource, ComputeScore(numCurrentSource(indexSource), numTotalSource(indexSource), timestamp)),
			scoreDestination.Assign(indexDestination, ComputeScore(numCurrentDestination(indexDestination), numTotalDestination(indexDestination), timestamp)),
		}));
	}
};
}
