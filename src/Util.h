#pragma once

#include <intrin.h>
#include <random>
#include <iostream>

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

	inline int Count(BitBoard b) {
		return (int) __popcnt64(b);
	}

	inline Square RandSquare(BitBoard b) {
		int idx = rand() % Count(b);
		Square sq;
		for (int i = 0; i <= idx; i++) {
			sq = PopWeak(&b);
		}
		return sq;
	}

	constexpr inline int PawnForward(Color c) {
		return c == White ? 8 : -8;
	}

	inline void DebugBitBoard(BitBoard b) {
		for (int y = 7; y >= 0; y--) {
			for (int x = 0; x < 8; x++) {
				std::cout << ((b & BitAt(x + y * 8)) ? '1' : '0');
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
}