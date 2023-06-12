#pragma once

#include <vector>
#include <string>

#include "Types.h"

#include "Moving.h"

namespace Mortan {
	struct Position {
		Move currMove;
		std::vector<Move> moves;

		Square passant;

		Castling castling[ColorCount];

		BitBoard board;
		Piece bySquare[SquareCount];
		BitBoard byColor[ColorCount];
		BitBoard byKind[PieceKindCount];

		Color opp;

		static Position FromFEN(const char *fen);
		static Position Default();

		int Preassure(Square, Color myColor); // apex search

		void DoPly(Ply); // and recalcs
	};
}