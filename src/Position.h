#pragma once

#include "Types.h"

namespace Mortan {
	struct PhysicalBoard {
		Piece bySquare[SquareCount];
		BitBoard byColor[ColorCount];
		BitBoard byKind[PieceKindCount];
	};

	struct MobilityBoard {
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
		File passant;
		PhysicalBoard physicalBoard;
		//MobilityBoard mobilityBoard;
		//ThreatBoard threatBoard;
		//WarBoard warBoard;

		Color opp;

		static Position Default();
		void Recalc(); // recalculate from `physicalBoard.bySquare`

		void DoMove(Square from, Square to); // and recalcs
	};
}