#pragma once

#include <vector>

#include "Types.h"

#include "Moving.h"

namespace Mortan {
	struct PhysicalBoard {
		Piece bySquare[SquareCount];
		BitBoard byColor[ColorCount];
		BitBoard byKind[PieceKindCount];
	};

	// "quite" moves
	struct MobilityBoard {
		BitBoard byColor[ColorCount];
	};

	struct AttackBoard {
		BitBoard byColor[ColorCount];
	};

	struct ThreatBoard {
		BitBoard byColor[ColorCount];
	};

	struct WarBoard {
		uint8_t byColor[ColorCount][SquareCount];
		int8_t weighted[SquareCount];
	};

	struct Position {
		Move currMove;
		std::vector<Move> moves;

		File passant;
		PhysicalBoard physicalBoard;

		Color opp;

		static Position Default();
		void Recalc(); // recalculate from `physicalBoard.bySquare`

		void DoPly(Ply ply); // and recalcs
	};
}