#pragma once

#include <vector>
#include <string>

#include "Types.h"

#include "Moving.h"

namespace Mortan {
	struct Position {
		Move currMove;
		std::vector<Move> moves;

		File passant;

		Castling castling[ColorCount];

		Piece bySquare[SquareCount];
		BitBoard byColor[ColorCount];
		BitBoard byKind[PieceKindCount];

		Color opp;

		static Position Default();
		static Position FromFEN(const char *fen);

		bool IsSafe(Square); // is there any attack on this square?

		void DoPly(Ply); // and recalcs
	};
}