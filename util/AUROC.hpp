// -----------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Rui LIU (@liurui39660)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -----------------------------------------------------------------------------
#pragma once

#include <algorithm>

/// @tparam T Type of array elements, should be a floating number type
/// @param yTrue Array of ground truth labels, 0.0 is negative, 1.0 is positive
/// @param yPred Array of predicted scores, can be of any range
/// @param n Number of elements in the array
/// @return AUROC/ROC-AUC score, range [0.0, 1.0]
template<class T>
double AUROC(const T* yTrue, const T* yPred, size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (std::isnan(yPred[i]) || std::isinf(yPred[i]) || yTrue[i] != 0 && yTrue[i] != 1)
			return -1;
	}

	const auto index = new size_t[n];
	for (size_t i = 0; i < n; i++) index[i] = i;
	// std::iota(index, index + n, 0); // Equivalent to above
	std::sort(index, index + n, [&](size_t a, size_t b) { return yPred[a] > yPred[b]; });
	const auto y = new double[n]; // Desc
	const auto z = new double[n]; // Desc
	for (size_t i = 0; i < n; i++) {
		y[i] = yTrue[index[i]];
		z[i] = yPred[index[i]];
	}

	const auto tp = y; // Reuse
	tp[0] = y[0];
	for (size_t i = 1; i < n; i++) {
		tp[i] = tp[i - 1] + y[i];
	} // Equivalent to below
	// std::partial_sum(y, y + n, tp);

	size_t top = 0; // # diff
	for (size_t i = 0; i < n - 1; i++) {
		if (z[i] != z[i + 1])
			index[top++] = i;
	}
	index[top++] = n - 1;
	n = top; // Size of y/z -> sizeof tps/fps
	delete[] z;

	const auto fp = new double[n];
	for (size_t i = 0; i < n; i++) {
		tp[i] = tp[index[i]];
		fp[i] = 1 + index[i] - tp[i];
	}
	delete[] index;

	const auto tp_diff = new double[n];
	const auto fp_diff = new double[n];
	for (size_t i = 1; i < n; i++) {
		tp_diff[i] = tp[i] - tp[i - 1];
		fp_diff[i] = fp[i] - fp[i - 1];
	} // Equivalent to below
	// std::adjacent_difference(tp, tp + n, tp_diff);
	// std::adjacent_difference(fp, fp + n, fp_diff);
	top = 1;
	for (size_t i = 1; i < n - 1; i++) {
		if (tp_diff[i] != tp_diff[i + 1] || fp_diff[i] != fp_diff[i + 1]) {
			tp[top] = tp[i];
			fp[top] = fp[i];
			top++;
		}
	}
	tp[top] = tp[n - 1];
	fp[top] = fp[n - 1];
	n = ++top; // Size of tp/fp -> size of optimized tp/fp 
	delete[] tp_diff;
	delete[] fp_diff;

	for (size_t i = 0; i < n; i++) {
		tp[i] /= tp[n - 1];
		fp[i] /= fp[n - 1];
	}

	double area = tp[0] * fp[0] / 2; // The first triangle from origin
	for (size_t i = 0; i < n - 1; i++) {
		area += (tp[i] + tp[i + 1]) * (fp[i + 1] - fp[i]) / 2;
	}

	delete[] tp;
	delete[] fp;

	return area;
}
