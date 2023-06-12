#pragma once

#include "Types.h"

namespace Mortan {
	struct Ply {
		Square from;
		Square to;

		bool enPassant;
		Castling castling;
		PieceKind promotion;
	};

	struct Move {
		Ply byColor[ColorCount];
	};
}