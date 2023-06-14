#pragma once

#include "Types.h"

namespace Mortan {
	struct Position;

	extern BitBoard pawnEyes[FileCount]; // attacks only
	extern BitBoard kingEyes[SquareCount];
	extern BitBoard knightEyes[SquareCount];
	extern BitBoard eyeRays[DirectionCount][SquareCount];

	void InitEyes();
	template<Direction dir>
	BitBoard RayMobilityWithBlockers(Square square, BitBoard pieces);

	BitBoard PieceQuites(const Position &, Square square);
	BitBoard PieceAttacks(const Position &, Square square);

	inline Piece PieceOf(Color color, PieceKind kind) {
		return (Piece) (color == White ? kind : kind + BKing);
	}

	inline Color ColorOf(Piece piece) {
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