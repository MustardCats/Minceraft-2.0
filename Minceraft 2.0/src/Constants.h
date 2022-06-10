#pragma once

#include "Include.h"

namespace pc {
	// chunk size
	constexpr short c_length = 16; // x
	constexpr short c_height = 256; // y
	constexpr short c_width = 16; // z

	enum class CardinalDirection {
		UP, DOWN, LEFT, RIGHT, FORWARD, BACKWARD
	};
}