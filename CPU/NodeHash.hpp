// -----------------------------------------------------------------------------
// Copyright 2020 Rui LIU
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

#include <algorithm>

namespace MIDAS::CPU {
struct NodeHash {
	// Fields
	// --------------------------------------------------------------------------------

	const int r, c;
	const int lenData;
	int* const param1;
	int* const param2;
	float* const data;
	constexpr static float infinity = std::numeric_limits<float>::infinity();

	// Methods
	// --------------------------------------------------------------------------------

	NodeHash() = delete;
	NodeHash& operator=(const NodeHash& b) = delete;

	NodeHash(int numRow, int numColumn):
		r(numRow),
		c(numColumn),
		lenData(r * c),
		param1(new int[r]),
		param2(new int[r]),
		data(new float[lenData]) {
		for (int i = 0; i < r; i++) {
			param1[i] = rand() + 1; // ×0 is not a good idea, see Hash()
			param2[i] = rand();
		}
		std::fill(data, data + lenData, 0);
	}

	NodeHash(const NodeHash& b):
		r(b.r),
		c(b.c),
		lenData(b.lenData),
		param1(new int[r]),
		param2(new int[r]),
		data(new float[lenData]) {
		std::copy(b.param1, b.param1 + r, param1);
		std::copy(b.param2, b.param2 + r, param2);
		std::copy(b.data, b.data + lenData, data);
	}

	~NodeHash() {
		delete[] param1;
		delete[] param2;
		delete[] data;
	}

	void MultiplyAll(float by = 0) const {
		std::for_each(data, data + lenData, [&](float& a) { a *= by; }); // Magic of vectorization
	}

	void Hash(int a, int* indexOut) const {
		for (int i = 0; i < r; i++) {
			indexOut[i] = (a * param1[i] + param2[i]) % c;
			indexOut[i] += i * c + (indexOut[i] < 0 ? c : 0);
		}
	}

	float operator()(const int* index) const {
		float least = infinity;
		for (int i = 0; i < r; i++)
			least = std::min(least, data[index[i]]);
		return least;
	}

	float Assign(const int* index, float to) const {
		for (int i = 0; i < r; i++)
			data[index[i]] = to;
		return to;
	}

	void Add(const int* index, float by = 1) const {
		for (int i = 0; i < r; i++)
			data[index[i]] += by;
	}
};
}
