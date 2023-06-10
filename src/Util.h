#pragma once

#include <intrin.h>

#include "Types.h"

namespace Mortan {
	inline constexpr uint64_t BitAt(int idx) {
		return 1ull << idx;
	}

	inline constexpr bool MoreThanOne(BitBoard b) {
		return b & (b - 1);
	}

#if defined(_MSC_VER)
	// idx of least significant bit
	inline Square WeakBit(BitBoard b) {
		unsigned long idx;
		_BitScanForward64(&idx, b);
		return (Square) idx;
	}

	// idx of most significant bit
	inline Square StrongBit(BitBoard b) {
		unsigned long idx;
		_BitScanReverse64(&idx, b);
		return (Square) idx;
	}
#else
#error "Ya no, use mvsc"
#endif

	inline Square PopWeak(BitBoard *b) {
		Square idx = WeakBit(*b);
		*b &= *b - 1;
		return idx;
	}

	inline Square PopStrong(BitBoard *b) {
		Square idx = StrongBit(*b);
		*b &= *b - 1;
		return idx;
	}
}