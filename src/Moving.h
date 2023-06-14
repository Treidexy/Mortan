#pragma once

#include "Types.h"

namespace Mortan {
	// for castle move king 2 squares
	// for passant move pawn 2 squares
	struct Ply {
		Square from;
		Square to;

		PieceKind promotion;
	};

	struct PlyInfo {
		Square from;
		Square to;

		Square passant;
		Castling castling;
		PieceKind promotion;
	};

	struct MoveInfo {
		PlyInfo byColor[ColorCount];
	};
}