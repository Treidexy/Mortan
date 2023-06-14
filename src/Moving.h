#pragma once

#include "Types.h"

namespace Mortan {
	// for castle move king 2 squares
	// for passant move pawn 2 squares
	struct Ply {
		Square from;
		Square to;

		PieceKind promotion;

		static const Ply None;
	};

	struct PlyInfo {
		Square from;
		Square to;

		Square passant;
		bool check;
		bool doubleCheck;
		bool enPassant; // you should be able to figure this out...
		Castling castling;
		PieceKind promotion;
	};

	struct MoveInfo {
		PlyInfo byColor[ColorCount];
	};
}