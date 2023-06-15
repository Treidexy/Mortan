#pragma once

#include "Moving.h"

namespace Mortan {
	struct Position;

	struct NoFish {
		static Ply MakeMove(const Position &);
	};
}