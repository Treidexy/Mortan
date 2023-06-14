#pragma once

#include "Moving.h"
#include "Types.h"

namespace Mortan {
	struct Position;

	struct NerdFish {
		static Ply MakeMove(const Position &);
	};
}