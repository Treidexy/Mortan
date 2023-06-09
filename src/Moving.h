#pragma once

#include "Types.h"

namespace Mortan {
	struct Ply {
		Square from;
		Square to;
	};

	struct Move {
		Ply byColor[ColorCount];
	};
}