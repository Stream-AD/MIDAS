#pragma once

#include <algorithm>
#include <unordered_map>

namespace RejectMIDAS::CPU {
struct EdgeList {
	/**
	 * No random, 1 row, dynamic number of columns.
	 * But exact same usage as hashing version.
	 * */

	// Fields
	// --------------------------------------------------------------------------------

	int lenData = 0;
	std::vector<float> data;
	std::vector<std::unordered_map<int, int>> hash;

	// Methods
	// --------------------------------------------------------------------------------

	EdgeList() = delete;
	EdgeList& operator=(const EdgeList& b) = delete;

	EdgeList(int numRow, int numColumn) { } // Two arguments are kept for compatibility

	void MultiplyAll(float by = 0) {
		std::for_each(data.begin(), data.end(), [&](float& a) { a *= by; });
	}

	void Hash(int a, int b, int indexOut[]) {
		if (a >= hash.size()) { // If a does not exist
			hash.resize(a + 1);
			indexOut[0] = hash[a][b] = data.size();
			data.push_back(0);
			lenData++;
		} else if (hash[a].contains(b)) {
			indexOut[0] = hash[a][b];
		} else {
			indexOut[0] = hash[a][b] = data.size();
			data.push_back(0);
			lenData++;
		}
	}

	float operator()(const int index[]) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		return data[index[0]];
	}

	float Assign(const int index[], float to) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		return data[index[0]] = to;
	}

	void Add(const int index[], float by = 1) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		data[index[0]] += by;
	}
};
}
