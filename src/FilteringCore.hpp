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
#include <algorithm>

#include "CountMinSketch.hpp"

namespace MIDAS {
struct FilteringCore {
	const float threshold;
	int timestampCurrent = 1;
	const float factor;
	int* const indexEdge; // Pre-compute the index to-be-modified, thanks to the same structure of CMSs
	int* const indexSource;
	int* const indexDestination;
	CountMinSketch numCurrentEdge, numTotalEdge, scoreEdge;
	CountMinSketch numCurrentSource, numTotalSource, scoreSource;
	CountMinSketch numCurrentDestination, numTotalDestination, scoreDestination;

	FilteringCore(int numRow, int numColumn, float threshold, float factor = 0.5):
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

	virtual ~FilteringCore() {
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
