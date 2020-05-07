#pragma once

#include <algorithm>

namespace RejectMIDAS::CPU {
struct NodeList {
	// Fields
	// --------------------------------------------------------------------------------

	int lenData = 0;
	std::vector<float> data;

	// Methods
	// --------------------------------------------------------------------------------

	NodeList() = delete;
	NodeList& operator=(const NodeList& b) = delete;

	NodeList(int numRow, int numColumn) { }

	void MultiplyAll(float by = 0) {
		std::for_each(data.begin(), data.end(), [&](float& a) { a *= by; });
	}

	void Hash(int a, int* indexOut) {
		if (a >= data.size()) {
			data.resize(a + 1);
			lenData = data.size();
		}
		indexOut[0] = a;
	}

	float operator()(const int* index) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		return data[index[0]];
	}

	float Assign(const int* index, float to) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		return data[index[0]] = to;
	}

	void Add(const int* index, float by = 1) {
		if (index[0] >= data.size())
			data.resize(index[0] + 1);
		data[index[0]] += by;
	}
};
}
