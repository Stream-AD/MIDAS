// -----------------------------------------------------------------------------
// Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// 	http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -----------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "CountMinSketch.hpp"

namespace MIDAS {
struct FilteringCore {
	const float threshold;
	int timestamp = 1;
	const float factor;
	const int lenData;
	int* const indexEdge; // Pre-compute the index to-be-modified, thanks to the Same-Layout Assumption
	int* const indexSource;
	int* const indexDestination;
	CountMinSketch numCurrentEdge, numTotalEdge, scoreEdge;
	CountMinSketch numCurrentSource, numTotalSource, scoreSource;
	CountMinSketch numCurrentDestination, numTotalDestination, scoreDestination;
	float timestampReciprocal = 0;
	bool* const shouldMerge;

	FilteringCore(int numRow, int numColumn, float threshold, float factor = 0.5):
		threshold(threshold),
		factor(factor),
		lenData(numRow * numColumn), // I assume all CMSs have same size, but Same-Layout Assumption is not that strict
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
		scoreDestination(numCurrentDestination),
		shouldMerge(new bool[numRow * numColumn]) { }

	virtual ~FilteringCore() {
		delete[] indexEdge;
		delete[] indexSource;
		delete[] indexDestination;
		delete[] shouldMerge;
	}

	static float ComputeScore(float a, float s, float t) {
		return s == 0 ? 0 : pow(a + s - a * t, 2) / (s * (t - 1)); // If t == 1, then s == 0, so no need to check twice
	}

	void ConditionalMerge(const float* current, float* total, const float* score) const {
		for (int i = 0; i < lenData; i++)
			shouldMerge[i] = score[i] < threshold;
		for (int i = 0, I = lenData; i < I; i++) // Vectorization
			total[i] += shouldMerge[i] * current[i] + (true - shouldMerge[i]) * total[i] * timestampReciprocal;
	}

	float operator()(int source, int destination, int timestamp) {
		if (this->timestamp < timestamp) {
			ConditionalMerge(numCurrentEdge.data, numTotalEdge.data, scoreEdge.data);
			ConditionalMerge(numCurrentSource.data, numTotalSource.data, scoreSource.data);
			ConditionalMerge(numCurrentDestination.data, numTotalDestination.data, scoreDestination.data);
			numCurrentEdge.MultiplyAll(factor);
			numCurrentSource.MultiplyAll(factor);
			numCurrentDestination.MultiplyAll(factor);
			timestampReciprocal = 1.f / (timestamp - 1); // So I can skip an if-statement
			this->timestamp = timestamp;
		}
		numCurrentEdge.Hash(indexEdge, source, destination);
		numCurrentEdge.Add(indexEdge);
		numCurrentSource.Hash(indexSource, source);
		numCurrentSource.Add(indexSource);
		numCurrentDestination.Hash(indexDestination, destination);
		numCurrentDestination.Add(indexDestination);
		return std::max({
			scoreEdge.Assign(indexEdge, ComputeScore(numCurrentEdge(indexEdge), numTotalEdge(indexEdge), timestamp)),
			scoreSource.Assign(indexSource, ComputeScore(numCurrentSource(indexSource), numTotalSource(indexSource), timestamp)),
			scoreDestination.Assign(indexDestination, ComputeScore(numCurrentDestination(indexDestination), numTotalDestination(indexDestination), timestamp)),
		});
	}
};
}
