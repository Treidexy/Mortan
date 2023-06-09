#pragma once

#include "Types.h"

namespace Mortan {
	struct Ply {
		Square from;
		Square to;

		PieceKind promotion;
	};

	struct Move {
		Ply byColor[ColorCount];
	};
}