#pragma once

#include <intrin.h>

#include "Types.h"

namespace Mortan {
	inline constexpr uint64_t BitAt(int idx) {
		return 1ull << idx;
	}

#if defined(_MSC_VER)
	// idx of least significant bit
	inline Square weakBit(BitBoard b) {
		unsigned long idx;
		_BitScanForward64(&idx, b);
		return (Square) idx;
	}

	// idx of most significant bit
	inline Square strongBit(BitBoard b) {
		unsigned long idx;
		_BitScanReverse64(&idx, b);
		return (Square) idx;
	}
#else
#error "Ya no, use mvsc"
#endif
}