#pragma once

#include "Moving.h"

namespace Mortan {
	struct Position;

	struct WeakBot {
		static Ply MakeMove(const Position &);
	};
}