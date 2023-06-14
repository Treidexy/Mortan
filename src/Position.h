#pragma once

#include <vector>
#include <string>

#include "Types.h"

#include "Moving.h"

namespace Mortan {
	struct Position {
		MoveInfo currMove;
		std::vector<MoveInfo> moves;

		Square passant;

		Castling castlingRights[ColorCount];

		BitBoard board;
		Piece bySquare[SquareCount];
		BitBoard byColor[ColorCount];
		BitBoard byKind[PieceKindCount];

		Color opp;

		static Position FromFEN(const char *fen);
		static Position Default();

		int Preassure(Square, Color myColor); // apex search

		bool DoPly(Ply); // returns true if the move was legal
	};
}