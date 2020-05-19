#pragma once

#include <cmath>

namespace MIDAS::CPU {
struct Option {
	/// Custom coefficient to scale CMSs when timestamp changes
	/// @param timestampCurrent The new timestamp, usually is an integer
	/// @param timestampPrevious The previous timestamp, usually is an integer
	/// @return A floating number, the coefficient
	float (* const ScalingCoefficient)(float, float) = [](float timestampCurrent, float timestampPrevious) {
		return std::pow(0.1f, timestampCurrent - timestampPrevious);
	};
};
}