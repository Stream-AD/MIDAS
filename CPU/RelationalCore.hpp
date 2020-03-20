#pragma once

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

	// Methods
	// --------------------------------------------------------------------------------

	explicit RelationalCore(int numRow, int numColumn, float thresholdRejection = 1e3f, float factorHashShrink = 0.5f) :
		factor(factorHashShrink),
		lenHash(numRow * numColumn),
		threshold(thresholdRejection),
		numCurrentEdge(numRow, numColumn),
		numTotalEdge(numCurrentEdge),
		scoreEdge(numCurrentEdge),
		numCurrentSource(numRow, numColumn),
		numTotalSource(numCurrentSource),
		scoreSource(numCurrentSource),
		numCurrentDestination(numRow, numColumn),
		numTotalDestination(numCurrentDestination),
		scoreDestination(numCurrentDestination) { }

	static float ComputeScore(float a, float s, float t) {
		return abs(s * (t - 1)) < 1e-4 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1));
	}

	float operator()(int source, int destination, int timestamp) {
		if (timestamp > timestampCurrent) {
#pragma omp parallel for
			for (const int i: numCurrentEdge.indexModified) {
				numTotalEdge.data[i] += scoreEdge.data[i] < threshold ?
					numCurrentEdge.data[i] : timestampCurrent - 1 ?
						numTotalEdge.data[i] / (timestampCurrent - 1) : 0;
			}
#pragma omp parallel for
			for (const int i: numCurrentSource.indexModified) {
				numTotalSource.data[i] += scoreSource.data[i] < threshold ?
					numCurrentSource.data[i] : timestampCurrent - 1 ?
						numTotalSource.data[i] / (timestampCurrent - 1) : 0;
			}
#pragma omp parallel for
			for (const int i: numCurrentDestination.indexModified) {
				numTotalDestination.data[i] += scoreDestination.data[i] < threshold ?
					numCurrentDestination.data[i] : timestampCurrent - 1 ?
						numTotalDestination.data[i] / (timestampCurrent - 1) : 0;
			}
			numCurrentEdge.MultiplyAll(factor);
			numCurrentSource.MultiplyAll(factor);
			numCurrentDestination.MultiplyAll(factor);
			timestampCurrent = timestamp;
		}
		numCurrentEdge.Add(source, destination);
		numCurrentSource.Add(source);
		numCurrentDestination.Add(destination);
		return std::log(1 + std::max({
			scoreEdge.Assign(source, destination, ComputeScore(numCurrentEdge(source, destination), numTotalEdge(source, destination), timestamp)),
			scoreSource.Assign(source, ComputeScore(numCurrentSource(source), numTotalSource(source), timestamp)),
			scoreDestination.Assign(destination, ComputeScore(numCurrentDestination(destination), numTotalDestination(destination), timestamp)),
		}));
	}
};
}
