#pragma once

#include "Types.h"

namespace Mortan {
	inline constexpr uint64_t BitAt(int idx) {
		return 1ull << idx;
	}
}