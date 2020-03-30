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
	const float Infinity = std::numeric_limits<float>::infinity();

	// Methods
	// --------------------------------------------------------------------------------

	EdgeHash() = delete;
	EdgeHash& operator=(const EdgeHash& b) = delete;

	EdgeHash(int numRow, int numColumn):
		r(numRow), c(numColumn), lenData(r * c), param1(new int[r]), param2(new int[r]), data(new float[lenData]) {
		for (int i = 0; i < r; i++) {
			param1[i] = rand() + 1; // ×0 is not a good idea, see Hash()
			param2[i] = rand();
		}
		std::fill(data, data + lenData, 0);
	}

	EdgeHash(const EdgeHash& b):
		r(b.r), c(b.c), lenData(b.lenData), param1(new int[r]), param2(new int[r]), data(new float[lenData]) {
		std::copy(b.param1, b.param1 + r, param1);
		std::copy(b.param2, b.param2 + r, param2);
		std::copy(b.data, b.data + lenData, data);
	}

	~EdgeHash() {
		delete[] param1;
		delete[] param2;
		delete[] data;
	}

	void MultiplyAll(float by) const {
		std::for_each(data, data + lenData, [&](float& a) { a *= by; }); // Magic of vectorization
	}

	void Clear() const {
		std::fill(data, data + lenData, 0); // Magic of vectorization
	}

	// If you prefer to hash once, use everywhere
	// --------------------------------------------------------------------------------

	void Hash(int a, int b, int indexOut[]) const {
		for (int i = 0; i < r; i++)
			indexOut[i] = i * c + abs((a + m * b) * param1[i] + param2[i]) % c;
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

	// If you wish to hash on-the-fly
	// --------------------------------------------------------------------------------

	int Hash(int a, int b, int r) const {
		return abs((a + m * b) * param1[r] + param2[r]) % c;
	}

	float operator()(int a, int b) const {
		float least = Infinity;
		for (int i = 0; i < r; i++)
			least = std::min(least, data[i * c + Hash(a, b, i)]);
		return least;
	}

	float Assign(int a, int b, float to) const {
		for (int i = 0; i < r; i++)
			data[i * c + Hash(a, b, i)] = to;
		return to;
	}

	void Add(int a, int b, float by = 1) const {
		for (int i = 0; i < r; i++)
			data[i * c + Hash(a, b, i)] += by;
	}
};
}
