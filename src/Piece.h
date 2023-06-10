#pragma once

#include "Types.h"

namespace Mortan {
	struct Position;

	extern BitBoard kingEyes[SquareCount];
	extern BitBoard knightEyes[SquareCount];
	extern BitBoard eyeRays[DirectionCount][SquareCount];

	void InitEyes();
	template<Direction dir>
	BitBoard RayMobilityWithBlockers(Square square, BitBoard pieces);

	inline Color PieceColor(Piece piece) {
		if (piece < BKing) {
			return White;
		}
		else if (piece < PieceCount) {
			return Black;
		} else {
			return ColorNone;
		}
	}

	inline PieceKind KindOf(Piece piece) {
		return (PieceKind) (piece < BKing ? piece : piece - BKing);
	}
}