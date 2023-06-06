#pragma once

#include "Types.h"

namespace Mortan {
	extern BitBoard kingEyes[SquareCount];
	extern BitBoard rookEyes[SquareCount];
	extern BitBoard bishopEyes[SquareCount];
	extern BitBoard knightEyes[SquareCount];

	void InitEyes();

	BitBoard PieceMobility(PieceKind, Square, BitBoard ally, BitBoard enemy, File passant);

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