#pragma once

#include <algorithm>

namespace RejectMIDAS::CPU {
struct EdgeHash {
	// Fields
	// --------------------------------------------------------------------------------

	const int r, c, m = 104729; // Yes, a magic number, I just pick a random prime
	const int lenData;
	int* const param1;
	int* const param2;
	float* const data;
	bool* const isModified;
	int* const indexModified;
	int headStack = 0;
	const float Infinity = std::numeric_limits<float>::infinity();

	// Methods
	// --------------------------------------------------------------------------------

	EdgeHash() = delete;
	EdgeHash& operator=(const EdgeHash& b) = delete;

	EdgeHash(int numRow, int numColumn):
		r(numRow),
		c(numColumn),
		lenData(r * c),
		param1(new int[r]),
		param2(new int[r]),
		data(new float[lenData]),
		isModified(new bool[lenData]),
		indexModified(new int[lenData]) {
		for (int i = 0; i < r; i++) {
			param1[i] = rand() + 1; // ×0 is not a good idea, see Hash()
			param2[i] = rand();
		}
		std::fill(data, data + lenData, 0);
		std::fill(isModified, isModified + lenData, false);
	}

	EdgeHash(const EdgeHash& b):
		r(b.r),
		c(b.c),
		lenData(b.lenData),
		param1(new int[r]),
		param2(new int[r]),
		data(new float[lenData]),
		isModified(new bool[lenData]),
		indexModified(new int[lenData]),
		headStack(b.headStack) {
		std::copy(b.param1, b.param1 + r, param1);
		std::copy(b.param2, b.param2 + r, param2);
		std::copy(b.data, b.data + lenData, data);
		std::copy(b.isModified, b.isModified + lenData, isModified);
		std::copy(b.indexModified, b.indexModified + lenData, indexModified);
	}

	~EdgeHash() {
		delete[] param1;
		delete[] param2;
		delete[] data;
		delete[] isModified;
		delete[] indexModified;
	}

	void MultiplyAll(float by) const {
		std::for_each(data, data + lenData, [&](float& a) { a *= by; }); // Magic of vectorization
	}

	void Clear() const {
		std::fill(data, data + lenData, 0); // Magic of vectorization
	}

	void MarkAsModified(const int index[]) {
		for (int i = 0; i < r; i++)
			if (!isModified[index[i]]) {
				isModified[index[i]] = true;
				indexModified[headStack++] = index[i];
			}
	}

	void ResetModifiedStatus() {
		while (headStack)
			isModified[indexModified[--headStack]] = false;
	}

	void Hash(int a, int b, int indexOut[]) const {
		for (int i = 0; i < r; i++) {
			indexOut[i] = ((a + m * b) * param1[i] + param2[i]) % c;
			indexOut[i] = i * c + indexOut[i] + (indexOut[i] < 0 ? c : 0);
		}
	}

	float operator()(const int index[]) const {
		float least = Infinity;
		for (int i = 0; i < r; i++)
			least = std::min(least, data[index[i]]);
		return least;
	}

	float Assign(const int index[], float to) const {
		for (int i = 0; i < r; i++)
			data[index[i]] = to;
		return to;
	}

	void Add(const int index[], float by = 1) const {
		for (int i = 0; i < r; i++)
			data[index[i]] += by;
	}
};
}
